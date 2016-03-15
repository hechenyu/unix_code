#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "semun.h"

#define	LOCK_PATH	"/tmp/svsemlock"
#define	MAX_TRIES	10

#define	SVSEM_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

int		semid, initflag, pipefd[2];
struct sembuf	postop, waitop;

void *
my_shm(size_t nbytes);

void
my_lock(int fd)
{
	int		oflag, i;
	union semun	arg;
	struct semid_ds	seminfo;
	key_t key;

	if ((key = ftok(LOCK_PATH, 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			LOCK_PATH, 0, strerror(errno));
		exit(1);
	}

	if (initflag == 0) {
		oflag = IPC_CREAT | IPC_EXCL | SVSEM_MODE;
		if ((semid = semget(key, 1, oflag)) >= 0) {
				/* 4success, we're the first so initialize */
			arg.val = 1;
			if (semctl(semid, 0, SETVAL, arg) == -1) {
				perror("semctl error");
				exit(1);
			}

		} else if (errno == EEXIST) {
				/* 4someone else has created; make sure it's initialized */
			if ((semid = semget(key, 1, SVSEM_MODE)) == -1) {
				perror("semget error");
				exit(1);
			}
			arg.buf = &seminfo;
			for (i = 0; i < MAX_TRIES; i++) {
				if (semctl(semid, 0, IPC_STAT, arg) == -1) {
					perror("semctl error");
					exit(1);
				}
				if (arg.buf->sem_otime != 0)
					goto init;
				if (write(pipefd[1], "", 1) != 1) { 	/* tell parent */
					perror("write error");
					exit(1);
				}
				sleep(1);
			}
			fprintf(stderr, "semget OK, but semaphore not initialized\n");
			exit(1);

		} else {
			perror("semget error");
			exit(1);
		}
init:
		initflag = 1;
		postop.sem_num = 0;		/* and init the two semop() structures */
		postop.sem_op  = 1;
		postop.sem_flg = SEM_UNDO;
		waitop.sem_num = 0;
		waitop.sem_op  = -1;
		waitop.sem_flg = SEM_UNDO;
	}
	if (semop(semid, &waitop, 1) == -1) {		/* down by 1 */
		perror("semop error");
		exit(1);
	}
}

void
my_unlock(int fd)
{
	if (semop(semid, &postop, 1) == -1) {		/* up by 1 */
		perror("semop error");
		exit(1);
	}
}

#define	SEQFILE	"seqno"		/* filename */

int
main(int argc, char **argv)
{
	int		fd = 0, stat, nconflicts;
	long	i, j, nproc;
	sem_t	*ptr;
	pid_t	pid;
	ssize_t	n;
	pid_t childpid;

	if (argc != 2) {
		fprintf(stderr, "usage: locksvsemrace1 <#processes>\n");
		exit(1);
	}
	nproc = atol(argv[1]);
	if (pipe(pipefd) == -1) {
		perror("pipe error");
		exit(1);
	}

	if ((ptr = my_shm(sizeof(sem_t))) == MAP_FAILED) {	/* create memory-based semaphore */
		perror("my_shm error");
		exit(1);
	}
	if (sem_init(ptr, 1, 0) == -1) {
		perror("sem_init error");
		exit(1);
	}

	for (j = 0; j < nproc; j++) {
		if ((childpid = fork()) == -1) {
			perror("fork error");
			exit(1);
		}
		if (childpid == 0) {
				/* 4child */
			if (sem_wait(ptr) == -1) {		/* wait for parent to start children */
				perror("sem_wait error");
				exit(1);
			}
			for (i = 0; i < 10; i++) {
				my_lock(fd);		/* lock the file */
				my_unlock(fd);		/* unlock the file */
			}
			exit(0);
		}
		/* parent loops around, creating next child */
	}
	for (j = 0; j < nproc; j++) {
		if (sem_post(ptr) == -1) {	/* start all the children */
			perror("sem_post error");
			exit(1);
		}
	}

	/* now just wait for all the children to finish */
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		;
	if (close(pipefd[1]) == -1) {
		perror("close error");
		exit(1);
	}
	nconflicts = 0;
	while ((n = read(pipefd[0], &stat, 1)) > 0) {
		nconflicts += n;
	}
	if (n == -1) {
		perror("read error");
		exit(1);
	}
	printf("%d conflicts\n", nconflicts);
	exit(0);
}
