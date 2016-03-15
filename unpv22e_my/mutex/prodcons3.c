#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define	MAXNITEMS	1000000
#define	MAXNTHREADS	100

int nitems;	/* read-only by producer and consumer */

struct {
	pthread_mutex_t	mutex;
	int buff[MAXNITEMS];
	int nput;
	int nval;
} shared = { PTHREAD_MUTEX_INITIALIZER };

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}

//void consume_wait(int i);
void *produce(void *), *consume(void *);

/* include main */
int main(int argc, char **argv)
{
	int i, nthreads, count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume;
	int err;

	if (argc != 3) {
		fprintf(stderr, "usage: prodcons3 <#items> <#threads>\n");
		exit(1);
	}
	nitems = min(atoi(argv[1]), MAXNITEMS);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);

	if ((err = pthread_setconcurrency(nthreads + 1)) != 0) {
		fprintf(stderr, "pthread_setconcurrency error: %s\n", strerror(err));
		exit(1);
	}

	/* 4create all producers and one consumer */
	for (i = 0; i < nthreads; i++) {
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
	for (i = 0; i < nthreads; i++) {
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

	exit(0);
}
/* end main */

void *produce(void *arg)
{
	int err;

	for ( ; ; ) {
		if ((err = pthread_mutex_lock(&shared.mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
			exit(1);
		}
		if (shared.nput >= nitems) {
			if ((err = pthread_mutex_unlock(&shared.mutex)) != 0) {
				fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
				exit(1);
			}
			return(NULL);		/* array is full, we're done */
		}
		shared.buff[shared.nput] = shared.nval;
		shared.nput++;
		shared.nval++;
		if ((err = pthread_mutex_unlock(&shared.mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
			exit(1);
		}
		*((int *) arg) += 1;
	}
	return(NULL);
}

/* include consume */
void consume_wait(int i)
{
	int err;
	for ( ; ; ) {
		if ((err = pthread_mutex_lock(&shared.mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
			exit(1);
		}
		if (i < shared.nput) {
			if ((err = pthread_mutex_unlock(&shared.mutex)) != 0) {
				fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
				exit(1);
			}
			return;			/* an item is ready */
		}
		if ((err = pthread_mutex_unlock(&shared.mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
			exit(1);
		}
	}
}

void *consume(void *arg)
{
	int i;

	for (i = 0; i < nitems; i++) {
		consume_wait(i);
		if (shared.buff[i] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i]);
	}
	return(NULL);
}
/* end consume */
