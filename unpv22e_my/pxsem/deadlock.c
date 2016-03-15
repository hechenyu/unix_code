/* include main */
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	NBUFF		2
#define	SEM_MUTEX	"/mutex"	/* these are args to px_ipc_name() */
#define	SEM_NEMPTY	"/nempty"
#define	SEM_NSTORED	"/nstored"

int nitems;		/* read-only by producer and consumer */
struct {	/* data shared by producer and consumer */
	int buff[NBUFF];
	sem_t *mutex, *nempty, *nstored;
} shared;

void *produce(void *), *consume(void *);

int main(int argc, char **argv)
{
	pthread_t tid_produce, tid_consume;
	int err;

	if (argc != 2) {
		fprintf(stderr, "usage: deadlock <#items>\n");
		exit(1);
	}
	nitems = atoi(argv[1]);

	/* 4create three semaphores */
	shared.mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, FILE_MODE, 1);
	if (shared.mutex == SEM_FAILED) {
		fprintf(stderr, "sem_open error for %s: %s\n", SEM_MUTEX, strerror(errno));
		exit(1);
	}

	shared.nempty = sem_open(SEM_NEMPTY, O_CREAT | O_EXCL, FILE_MODE, NBUFF);
	if (shared.nempty == SEM_FAILED) {
		fprintf(stderr, "sem_open error for %s: %s\n", SEM_NEMPTY, strerror(errno));
		exit(1);
	}

	shared.nstored = sem_open(SEM_NSTORED, O_CREAT | O_EXCL, FILE_MODE, 0);
	if (shared.nstored == SEM_FAILED) {
		fprintf(stderr, "sem_open error for %s: %s\n", SEM_NSTORED, strerror(errno));
		exit(1);
	}

#ifdef __USE_UNIX98
	if ((err = pthread_setconcurrency(2)) != 0) {
		fprintf(stderr, "pthread_setconcurrency error: %s\n", strerror(err));
		exit(1);
	}
#endif
	if ((err = pthread_create(&tid_produce, NULL, produce, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_create(&tid_consume, NULL, consume, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}

	if ((err = pthread_join(tid_produce, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_join(tid_consume, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}

	if (sem_unlink(SEM_MUTEX) == -1) {
		perror("sem_unlink error");
		exit(1);
	}
	if (sem_unlink(SEM_NEMPTY) == -1) {
		perror("sem_unlink error");
		exit(1);
	}
	if (sem_unlink(SEM_NSTORED) == -1) {
		perror("sem_unlink error");
		exit(1);
	}
	exit(0);
}
/* end main */

/* include prodcons */
void *produce(void *arg)
{
	int i;

	for (i = 0; i < nitems; i++) {
		printf("prod: calling sem_wait(nempty)\n");
		/* wait for at least 1 empty slot */
		if (sem_wait(shared.nempty) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		printf("prod: got sem_wait(nempty)\n");
		printf("prod: calling sem_wait(mutex)\n");
		if (sem_wait(shared.mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		printf("prod: got sem_wait(mutex), storing %d\n", i);
		shared.buff[i % NBUFF] = i;	/* store i into circular buffer */
		if (sem_post(shared.mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		/* 1 more stored item */
		if (sem_post(shared.nstored) == -1) {
			perror("sem_post error");
			exit(1);
		}
	}
	return(NULL);
}

void *consume(void *arg)
{
	int i;

	for (i = 0; i < nitems; i++) {
		printf("cons: calling sem_wait(mutex)\n");
		if (sem_wait(shared.mutex) == -1) {
			fprintf(stderr, "sem_wait error: %s\n", strerror(errno));
			exit(1);
		}
		printf("cons: got sem_wait(mutex)\n");
		printf("cons: calling sem_wait(nstored)\n");
		/* wait for at least 1 stored item */
		if (sem_wait(shared.nstored) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		printf("cons: got sem_wait(nstored)\n");
		printf("cons: fetched %d\n", shared.buff[i % NBUFF]);
		if (sem_post(shared.mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		/* 1 more empty slot */
		if (sem_post(shared.nempty) == -1 ) {
			perror("sem_post error");
			exit(1);
		}
	}
	printf("\n");
	return(NULL);
}
/* end prodcons */
