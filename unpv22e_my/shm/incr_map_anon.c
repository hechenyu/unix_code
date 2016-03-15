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

/* include diff */
int
main(int argc, char **argv)
{
	int		i, nloop;
	int		*ptr;
	sem_t	*mutex;
	pid_t childpid;

	if (argc != 2) {
		fprintf(stderr, "usage: incr_map_anon <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);

		/* 4map into memory */
	if ((ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
			   MAP_SHARED | MAP_ANON, -1, 0)) == MAP_FAILED) {
		perror("mmap error");
		exit(1);
	}
/* end diff */

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
			printf("child: %d\n", (*ptr)++);
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
		printf("parent: %d\n", (*ptr)++);
		if (sem_post(mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
	}
	exit(0);
}
