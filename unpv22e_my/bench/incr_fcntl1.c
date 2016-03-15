/* Although this program compiles and runs for 1 thread, you cannot run
   it with more than 1 thread, since fcntl locks are between processes,
   not threads */

/* include main */
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "lock.h"

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	MAXNTHREADS	100

int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}

int		nloop;

struct {
  int	fd;
  long	counter;
} shared;

void	*incr(void *);

int
start_time(void);

double
stop_time(void);

int
main(int argc, char **argv)
{
	int		i, nthreads;
	char	*pathname;
	pthread_t	tid[MAXNTHREADS];
	int err;
	double st;	// stop time

	if (argc != 4) {
		fprintf(stderr, "usage: incr_fcntl1 <pathname> <#loops> <#threads>\n");
		exit(1);
	}
	pathname = argv[1];
	nloop = atoi(argv[2]);
	nthreads = min(atoi(argv[3]), MAXNTHREADS);

		/* 4create the file and obtain write lock */
	if ((shared.fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", pathname, strerror(errno));
		exit(1);
	}
	if (writew_lock(shared.fd, 0, SEEK_SET, 0) == -1) {
		perror("writew_lock error");
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
		/* 4start the timer and release the write lock */
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	if (un_lock(shared.fd, 0, SEEK_SET, 0) == -1) {
		perror("un_lock error");
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
	if (unlink(pathname) == -1) {
		perror("unlink error");
		exit(1);
	}

	exit(0);
}
/* end main */

/* include incr */
void *
incr(void *arg)
{
	int		i;

	for (i = 0; i < nloop; i++) {
		if (writew_lock(shared.fd, 0, SEEK_SET, 0) == -1) {
			perror("writew_lock error");
			exit(1);
		}
		shared.counter++;
		if (un_lock(shared.fd, 0, SEEK_SET, 0) == -1) {
			perror("un_lock error");
			exit(1);
		}
	}
	return(NULL);
}
/* end incr */
