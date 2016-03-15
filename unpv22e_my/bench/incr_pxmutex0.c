/* include main */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define	MAXNTHREADS	100

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}

int		nloop;

struct {
  pthread_mutex_t	mutex;
  long	counter;
} shared = { PTHREAD_MUTEX_INITIALIZER };

void	*incr(void *);

int
start_time(void);

double
stop_time(void);

int
main(int argc, char **argv)
{
	int		i, nthreads;
	pthread_t	tid[MAXNTHREADS];
	int err;
	double st;	// stop time

	if (argc != 3) {
		fprintf(stderr, "usage: incr_pxmutex0 <#loops> <#threads>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);

		/* 4lock the mutex */
	if ((err = pthread_mutex_lock(&shared.mutex)) != 0) {
		fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
		exit(1);
	}

		/* 4create all the threads */
	if ((err = pthread_setconcurrency(nthreads)) != 0) {
		fprintf(stderr, "pthread_setconcurrency error: %s\n", strerror(err));
		exit(1);
	}
	for (i = 0; i < nthreads; i++) {
		if ((err = pthread_create(&tid[i], NULL, incr, NULL)) != 0) {
			fprintf(stderr, "pthread_create error: %s\n", strerror(err));
			exit(1);
		}
	}
		/* 4start the timer and unlock the mutex */
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	if ((err = pthread_mutex_unlock(&shared.mutex)) != 0) {
		fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
		exit(1);
	}

		/* 4wait for all the threads */
	for (i = 0; i < nthreads; i++) {
		if ((err = pthread_join(tid[i], NULL)) != 0) {
			fprintf(stderr, "pthread_join error: %s\n", strerror(err));
			exit(1);
		}
	}
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("microseconds: %.0f usec\n", st);
	if (shared.counter != nloop * nthreads)
		printf("error: counter = %ld\n", shared.counter);

	exit(0);
}
/* end main */

/* include incr */
void *
incr(void *arg)
{
	int		i;
	int		err;

	if ((err = pthread_mutex_lock(&shared.mutex)) != 0) { /* wait to start */
		fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_mutex_unlock(&shared.mutex)) != 0) { /* then release */
		fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
		exit(1);
	}
	for (i = 0; i < nloop; i++) {
		shared.counter++;
	}
	return(NULL);
}
/* end incr */
