#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "semun.h"

#define	SVSEM_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV semaphores */

#define	LOCK_PATH	"/tmp/svsemlock"
#define	MAX_TRIES	10

int		semid, initflag;
struct sembuf	postop, waitop;

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
