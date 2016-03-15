#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define	NLOOP 5000

int				counter;		/* incremented by threads */
pthread_mutex_t	counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void	*doit(void *);

int
main(int argc, char **argv)
{
	pthread_t	tidA, tidB;
	int n;

	if ((n = pthread_create(&tidA, NULL, &doit, NULL)) != 0) {
		errno = n;
		perror("pthread_create error");
		exit(1);
	}
	if ((n = pthread_create(&tidB, NULL, &doit, NULL)) != 0) {
		errno = n;
		perror("pthread_create error");
		exit(1);
	}

		/* 4wait for both threads to terminate */
	if ((n = pthread_join(tidA, NULL)) != 0) {
		errno = n;
		perror("pthread_join error");
		exit(1);
	}
	if ((n = pthread_join(tidB, NULL)) != 0) {
		errno = n;
		perror("pthread_join error");
		exit(1);
	}

	exit(0);
}

void *
doit(void *vptr)
{
	int		i, val;
	int		n;

	/*
	 * Each thread fetches, prints, and increments the counter NLOOP times.
	 * The value of the counter should increase monotonically.
	 */

	for (i = 0; i < NLOOP; i++) {
		if ((n = pthread_mutex_lock(&counter_mutex)) != 0) {
			errno = n;
			perror("pthread_mutex_lock error");
			exit(1);
		}

		val = counter;
		printf("%d: %d\n", pthread_self(), val + 1);
		counter = val + 1;

		if ((n = pthread_mutex_unlock(&counter_mutex)) != 0) {
			errno = n;
			perror("pthread_mutex_unlock error");
			exit(1);
		}
	}

	return(NULL);
}
