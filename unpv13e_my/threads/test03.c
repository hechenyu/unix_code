/* test pthread_cond_timedwait() */
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int				ndone;
pthread_mutex_t	ndone_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	ndone_cond = PTHREAD_COND_INITIALIZER;

void *
myfunc(void *ptr)
{
	int		val;

	sleep(100);		/* do not set ndone, do not cond_signal() */
	return(NULL);
}

int
main(int argc, char **argv)
{
	pthread_t		tid;
	int				n, val;
	struct timeval	tv;
	struct timespec	ts;

	if ( (n = pthread_create(&tid, NULL, myfunc, &val)) != 0) {
		errno = n;
		perror("pthread_create error");
		exit(1);
	}

	if (gettimeofday(&tv, NULL) < 0) {
		perror("gettimeofday error");
		exit(1);
	}
	ts.tv_sec  = tv.tv_sec + 5;		/* 5 seconds in future */
	ts.tv_nsec = tv.tv_usec * 1000;

	if ( (n = pthread_mutex_lock(&ndone_mutex)) != 0) {
		errno = n;
		perror("pthread_mutex_lock error");
		exit(1);
	}
	while (ndone == 0)
		if ( (n = pthread_cond_timedwait(&ndone_cond, &ndone_mutex, &ts)) != 0) {
			if (n == ETIME) {
				fprintf(stderr, "timewait timed out\n");
				exit(1);
			}
			errno = n;
			perror("pthread_cond_timedwait error");
			exit(1);
		}

	if ( (n = pthread_mutex_unlock(&ndone_mutex)) != 0) {
		errno = n;
		perror("pthread_mutex_unlock error");
		exit(1);
	}

	exit(0);
}
