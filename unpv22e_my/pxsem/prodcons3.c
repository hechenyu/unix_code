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

#define	NBUFF		10
#define	MAXNTHREADS	100

int nitems, nproducers;		/* read-only by producer and consumer */

struct {	/* data shared by producers and consumer */
	int buff[NBUFF];
	int nput;
	int nputval;
	sem_t mutex, nempty, nstored;		/* semaphores, not pointers */
} shared;

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}

void *produce(void *), *consume(void *);

int main(int argc, char **argv)
{
	int i, count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume;
	int err;

	if (argc != 3) {
		fprintf(stderr, "usage: prodcons3 <#items> <#producers>\n");
		exit(1);
	}
	nitems = atoi(argv[1]);
	nproducers = min(atoi(argv[2]), MAXNTHREADS);

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

	/* 4create all producers and one consumer */
#ifdef __USE_UNIX98
	if ((err = pthread_setconcurrency(nproducers + 1)) != 0) {
		fprintf(stderr, "pthread_setconcurrency error: %s\n", strerror(err));
		exit(1);
	}
#endif
	for (i = 0; i < nproducers; i++) {
		count[i] = 0;
		if ((err = pthread_create(&tid_produce[i], NULL, produce, &count[i])) != 0) {
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}
	}
	if ((err = pthread_create(&tid_consume, NULL, consume, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}

	/* 4wait for all producers and the consumer */
	for (i = 0; i < nproducers; i++) {
		if ((err = pthread_join(tid_produce[i], NULL)) != 0) {
			fprintf(stderr, "pthread_join error: %s\n", strerror(err));
			exit(1);
		}
		printf("count[%d] = %d\n", i, count[i]);	
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
/* end main */

/* include produce */
void *produce(void *arg)
{
	for ( ; ; ) {
		/* wait for at least 1 empty slot */
		if (sem_wait(&shared.nempty) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		if (sem_wait(&shared.mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}

		if (shared.nput >= nitems) {
			if (sem_post(&shared.nempty) == -1 ) {
				perror("sem_post error");
				exit(1);
			}
			if (sem_post(&shared.mutex) == -1) {
				perror("sem_post error");
				exit(1);
			}
			return(NULL);			/* all done */
		}

		shared.buff[shared.nput % NBUFF] = shared.nputval;
		shared.nput++;
		shared.nputval++;

		if (sem_post(&shared.mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
		/* 1 more stored item */
		if (sem_post(&shared.nstored) == -1) {
			perror("sem_post error");
			exit(1);
		}
		*((int *) arg) += 1;
	}
}
/* end produce */

/* include consume */
void *consume(void *arg)
{
	int	i;

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
			printf("error: buff[%d] = %d\n", i, shared.buff[i % NBUFF]);

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
/* end consume */
