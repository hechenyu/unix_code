/* include main */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define	MAXNPROC	100

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}

int		nloop;

struct shared {
  pthread_mutex_t	mutex;
  long	counter;
} *shared;		/* pointer; actual structure in shared memory */

void	*incr(void *);

void *
my_shm(size_t nbytes);

int
start_time(void);

double
stop_time(void);

int
main(int argc, char **argv)
{
	int		i, nprocs;
	pid_t	childpid[MAXNPROC];
	pthread_mutexattr_t	mattr;
	int err;
	double st;	// stop time

	if (argc != 3) {
		fprintf(stderr, "usage: incr_pxmutex5 <#loops> <#processes>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);
	nprocs = min(atoi(argv[2]), MAXNPROC);

		/* 4get shared memory for parent and children */
	if ((shared = my_shm(sizeof(struct shared))) == MAP_FAILED) {
		perror("my_shm error");
		exit(1);
	}

		/* 4initialize the mutex and lock it */
	if ((err = pthread_mutexattr_init(&mattr)) != 0) {
		fprintf(stderr, "pthread_mutexattr_init error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED)) != 0) {
		fprintf(stderr, "pthread_mutexattr_setpshared error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_mutex_init(&shared->mutex, &mattr)) != 0) {
		fprintf(stderr, "pthread_mutex_init error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_mutexattr_destroy(&mattr)) != 0) {
		fprintf(stderr, "pthread_mutexattr_destroy error: %s\n", strerror(err));
		exit(1);
	}
	if ((err = pthread_mutex_lock(&shared->mutex)) != 0) {
		fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
		exit(1);
	}

		/* 4create all the children */
	for (i = 0; i < nprocs; i++) {
		if ( (childpid[i] = fork()) == -1) {
			perror("fork error");
			exit(1);
		} else if (childpid[i] == 0) {
			incr(NULL);
			exit(0);
		}
	}
		/* 4parent: start the timer and unlock the mutex */
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	if ((err = pthread_mutex_unlock(&shared->mutex)) != 0) {
		fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
		exit(1);
	}

		/* 4wait for all the children */
	for (i = 0; i < nprocs; i++) {
		if (waitpid(childpid[i], NULL, 0) == -1) {
			perror("waitpid error");
			exit(1);
		}
	}
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("microseconds: %.0f usec\n", st);
	if (shared->counter != nloop * nprocs)
		printf("error: counter = %ld\n", shared->counter);

	exit(0);
}
/* end main */

/* include incr */
void *
incr(void *arg)
{
	int		i;
	int		err;

	for (i = 0; i < nloop; i++) {
		if ((err = pthread_mutex_lock(&shared->mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_lock error: %s\n", strerror(err));
			exit(1);
		}
		shared->counter++;
		if ((err = pthread_mutex_unlock(&shared->mutex)) != 0) {
			fprintf(stderr, "pthread_mutex_unlock error: %s\n", strerror(err));
			exit(1);
		}
	}
	return(NULL);
}
/* end incr */
