#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define	MAXNITEMS	1000000
#define	MAXNTHREADS	100

/* globals shared by threads */
int nitems;	/* read-only by producer and consumer */
int buff[MAXNITEMS];
struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	int nput;
	int nval;
	int nready;
} nready = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER };

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}

void *produce(void *), *consume(void *);

/* include main */
int main(int argc, char **argv)
{
	int i, nthreads, count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume;
	int err;

	if (argc != 3) {
		fprintf(stderr, "usage: prodcons5 <#items> <#threads>\n");
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

	/* wait for all producers and the consumer */
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
		if ((err = pthread_mutex_lock(&nready.mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
			exit(1);
		}
		if (nready.nput >= nitems) {
			if ((err = pthread_mutex_unlock(&nready.mutex)) != 0) {
				fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
				exit(1);
			}
			return(NULL);		/* array is full, we're done */
		}
		buff[nready.nput] = nready.nval;
		nready.nput++;
		nready.nval++;
		nready.nready++;
		if ((err = pthread_cond_signal(&nready.cond)) != 0) {
			fprintf(stderr, "pthread_cond_signal error: %s\n", strerror(err));
			exit(1);
		}
		if ((err = pthread_mutex_unlock(&nready.mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
			exit(1);
		}
		*((int *) arg) += 1;
	}
}

/* include consume */
void *consume(void *arg)
{
	int i, err;

	for (i = 0; i < nitems; i++) {
		if ((err = pthread_mutex_lock(&nready.mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
			exit(1);
		}
		while (nready.nready == 0) {
			if ((err = pthread_cond_wait(&nready.cond, &nready.mutex)) != 0) {
				fprintf(stderr, "pthread_cond_wait error: %s\n", strerror(err));
				exit(1);
			}
		}
		nready.nready--;
		if ((err = pthread_mutex_unlock(&nready.mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
			exit(1);
		}

		if (buff[i] != i)
			printf("buff[%d] = %d\n", i, buff[i]);
	}
	return(NULL);
}
/* end consume */
