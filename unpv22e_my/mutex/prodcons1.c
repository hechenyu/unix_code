/* include main */
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

void *produce(void *), *consume(void *);

int main(int argc, char **argv)
{
	int i, nthreads, count[MAXNTHREADS];
	pthread_t tid_produce[MAXNTHREADS], tid_consume;
	int err;

	if (argc != 3) {
		fprintf(stderr, "usage: prodcons1 <#items> <#threads>\n");
		exit(1);
	}
	nitems = min(atoi(argv[1]), MAXNITEMS);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);

	if ((err = pthread_setconcurrency(nthreads)) != 0) {
		fprintf(stderr, "pthread_setconcurrency error: %s\n", strerror(err));
		exit(1);
	}

	for (i = 0; i < nthreads; i++) {
		count[i] = 0;
		if ((err = pthread_create(&tid_produce[i], NULL, produce, &count[i])) != 0) {
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}
	}

	for (i = 0; i < nthreads; i++) {
		if ((err = pthread_join(tid_produce[i], NULL)) != 0) {
			fprintf(stderr, "pthread_join error: %s\n", strerror(err));
			exit(1);
		}
		printf("count[%d] = %d\n", i, count[i]);	
	}

	if ((err = pthread_create(&tid_consume, NULL, consume, NULL)) != 0) {
		fprintf(stderr, "pthread_create error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_join(tid_consume, NULL)) != 0) {
		fprintf(stderr, "pthread_join error: %s\n", strerror(err));
		exit(1);
	}

	exit(0);
}
/* end main */

/* include produce */
void *produce(void *arg)
{
	for ( ; ; ) {
		if (shared.nput >= nitems) {
			return(NULL);		/* array is full, we're done */
		}
		shared.buff[shared.nput] = shared.nval;
		shared.nput++;
		shared.nval++;
		*((int *) arg) += 1;
	}
}

void *consume(void *arg)
{
	int	i;

	for (i = 0; i < nitems; i++) {
		if (shared.buff[i] != i)
			printf("buff[%d] = %d\n", i, shared.buff[i]);
	}
	return(NULL);
}
/* end produce */
