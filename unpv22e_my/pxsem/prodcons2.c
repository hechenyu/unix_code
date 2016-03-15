#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define	NBUFF	 10

int nitems;		/* read-only by producer and consumer */
struct {	/* data shared by producer and consumer */
	int buff[NBUFF];
	sem_t mutex, nempty, nstored;		/* semaphores, not pointers */
} shared;

void *produce(void *), *consume(void *);

int main(int argc, char **argv)
{
	pthread_t tid_produce, tid_consume;
	int err;

	if (argc != 2) {
		fprintf(stderr, "usage: prodcons2 <#items>\n");
		exit(1);
	}
	nitems = atoi(argv[1]);

	/* 4initialize three semaphores */
	if (sem_init(&shared.mutex, 0, 1) == -1) {
		perror("sem_init error");
		exit(1);
	}
	if (sem_init(&shared.nempty, 0, NBUFF) == -1) {
		perror("sem_init error");
		exit(1);
	}
	if (sem_init(&shared.nstored, 0, 0) == -1) {
		perror("sem_init error");
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

	/* 4wait for the two threads */
	if ((err = pthread_join(tid_produce, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_join(tid_consume, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}

	/* 4remove the semaphores */
	if (sem_destroy(&shared.mutex) == -1) {
		perror("sem_destroy error");
		exit(1);
	}
	if (sem_destroy(&shared.nempty) == -1) {
		perror("sem_destroy error");
		exit(1);
	}
	if (sem_destroy(&shared.nstored) == -1) {
		perror("sem_destroy error");
		exit(1);
	}
	exit(0);
}

/* include prodcons */
void *produce(void *arg)
{
	int i;

	for (i = 0; i < nitems; i++) {
		/* wait for at least 1 empty slot */
		if (sem_wait(&shared.nempty) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		if (sem_wait(&shared.mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		shared.buff[i % NBUFF] = i;	/* store i into circular buffer */
		if (sem_post(&shared.mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		/* 1 more stored item */
		if (sem_post(&shared.nstored) == -1) {
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
		/* wait for at least 1 stored item */
		if (sem_wait(&shared.nstored) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		if (sem_wait(&shared.mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		if (shared.buff[i % NBUFF] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
		if (sem_post(&shared.mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		/* 1 more empty slot */
		if (sem_post(&shared.nempty) == -1 ) {
			perror("sem_post error");
			exit(1);
		}
	}
	return(NULL);
}
/* end prodcons */

