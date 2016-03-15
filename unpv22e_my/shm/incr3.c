#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define	SEM_NAME	"/mysem"
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

struct shared {
  sem_t	mutex;		/* the mutex: a Posix memory-based semaphore */
  int	count;		/* and the counter */
} shared;

int
main(int argc, char **argv)
{
	int		fd, i, nloop;
	struct shared 	*ptr;
	pid_t childpid;

	if (argc != 3) {
		fprintf(stderr, "usage: incr2 <pathname> <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[2]);

		/* 4open file, initialize to 0, map into memory */
	if ((fd = open(argv[1], O_RDWR | O_CREAT, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if (write(fd, &shared, sizeof(struct shared)) != sizeof(struct shared)) {
		perror("write error");
		exit(1);
	}
	if ((ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}

		/* 4create, initialize, and unlink semaphore */
	if (sem_init(&ptr->mutex, 1, 1) == -1) {
		perror("sem_init error");
		exit(1);
	}

	setbuf(stdout, NULL);	/* stdout is unbuffered */
	if ((childpid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {		/* child */
		for (i = 0; i < nloop; i++) {
			if (sem_wait(&ptr->mutex) == -1) {
				perror("sem_wait error");
				exit(1);
			}
			printf("child: %d\n", ptr->count++);
			if (sem_post(&ptr->mutex) == -1) {
				perror("sem_post error");
				exit(1);
			}
		}
		exit(0);
	}

		/* 4parent */
	for (i = 0; i < nloop; i++) {
		if (sem_wait(&ptr->mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		printf("parent: %d\n", ptr->count++);
		if (sem_post(&ptr->mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
	}
	exit(0);
}
