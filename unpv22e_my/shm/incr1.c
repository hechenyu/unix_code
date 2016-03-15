#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define	SEM_NAME	"/mysem"
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int		count = 0;

int
main(int argc, char **argv)
{
	int		i, nloop;
	sem_t	*mutex;
	pid_t childpid;

	if (argc != 2) {
		fprintf(stderr, "usage: incr1 <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);

		/* 4create, initialize, and unlink semaphore */
	if ((mutex = sem_open(SEM_NAME, O_CREAT | O_EXCL, FILE_MODE, 1)) == SEM_FAILED) {
		fprintf(stderr, "sem_open error for %s: %s\n", SEM_NAME, strerror(errno));
		exit(1);
	}
	if (sem_unlink(SEM_NAME) == -1) {
		perror("sem_unlink error");
		exit(1);
	}

	setbuf(stdout, NULL);	/* stdout is unbuffered */
	if ((childpid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {		/* child */
		for (i = 0; i < nloop; i++) {
			if (sem_wait(mutex) == -1) {
				perror("sem_wait error");
				exit(1);
			}
			printf("child: %d\n", count++);
			if (sem_post(mutex) == -1) {
				perror("sem_post error");
				exit(1);
			}
		}
		exit(0);
	}

		/* 4parent */
	for (i = 0; i < nloop; i++) {
		if (sem_wait(mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		printf("parent: %d\n", count++);
		if (sem_post(mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
	}
	exit(0);
}
