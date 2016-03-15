/* include main */
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define	MAXNPROC	100
#define	NAME	"/incr_pxsem6"
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}

int		nloop;

struct shared {
  sem_t	*mutex;
  long	counter;
} *shared;		/* actual structure is stored in shared memory */

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
	double st;	// stop time

	if (argc != 3) {
		fprintf(stderr, "usage: incr_pxsem6 <#loops> <#processes>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);
	nprocs = min(atoi(argv[2]), MAXNPROC);

		/* 4get shared memory for parent and children */
	if ((shared = my_shm(sizeof(struct shared))) == MAP_FAILED) {
		perror("my_shm error");
		exit(1);
	}

		/* 4initialize named semaphore to 0 */
	sem_unlink(NAME);		/* error OK */
	if ((shared->mutex = sem_open(NAME, O_CREAT | O_EXCL, FILE_MODE, 0)) == SEM_FAILED) {
		fprintf(stderr, "sem_open error for %s: %s\n", NAME, strerror(errno));
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
		/* 4parent: start the timer and release the semaphore */
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	if (sem_post(shared->mutex) == -1) {
		perror("sem_post error");
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

	if (sem_unlink(NAME) == -1) {
		perror("sem_unlink error");
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
		if (sem_wait(shared->mutex) == -1) {
			perror("sem_wait error");
			exit(1);
		}
		shared->counter++;
		if (sem_post(shared->mutex) == -1) {
			perror("sem_post error");
			exit(1);
		}
	}
	return(NULL);
}
/* end incr */
