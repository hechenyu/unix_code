#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define	NAME	"/testeintr"
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

static void sig_alrm(int);

int main(int argc, char **argv)
{
	sem_t *sem1, sem2;

	/* 4first test a named semaphore */
	sem_unlink(NAME);
	sem1 = sem_open(NAME, O_RDWR | O_CREAT | O_EXCL, FILE_MODE, 0);
	if (sem1 == SEM_FAILED) {
		fprintf(stderr, "sem_open error for %s: %s\n", NAME, strerror(errno));
		exit(1);
	}

	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	alarm(2);
	if (sem_wait(sem1) == 0) {
		printf("sem_wait returned 0?\n");
	} else {
		perror("sem_wait error");
		exit(1);
	}
	if (sem_close(sem1) == -1) {
		perror("sem_close error");
		exit(1);
	}

	/* 4now a memory-based semaphore with process scope */
	if (sem_init(&sem2, 1, 0) == -1) {
		perror("sem_init error");
		exit(1);
	}
	alarm(2);
	if (sem_wait(&sem2) == 0) {
		printf("sem_wait returned 0?\n");
	} else {
		perror("sem_wait error");
		exit(1);
	}
	if (sem_destroy(&sem2) == -1) {
		perror("sem_destroy error");
		exit(1);
	}

	exit(0);
}

static void sig_alrm(int signo)
{
	printf("SIGALRM caught\n");
	return;
}

