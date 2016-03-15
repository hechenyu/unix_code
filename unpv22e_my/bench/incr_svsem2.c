/* include main */
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "semun.h"

#define	SVSEM_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV semaphores */

#define	MAXNTHREADS	100

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}

int		nloop;

struct {
  int	semid;
  long	counter;
} shared;

struct sembuf	postop, waitop;

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
	union semun	arg;
	int err;
	double st;	// stop time

	if (argc != 3) {
		fprintf(stderr, "usage: incr_svsem2 <#loops> <#threads>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);
	nthreads = min(atoi(argv[2]), MAXNTHREADS);

		/* 4create semaphore and initialize to 0 */
	if ((shared.semid = semget(IPC_PRIVATE, 1, IPC_CREAT | SVSEM_MODE)) == -1) {
		perror("semget error");
		exit(1);
	}
	arg.val = 0;
	if (semctl(shared.semid, 0, SETVAL, arg) == -1) {
		perror("semctl error");
		exit(1);
	}
	postop.sem_num = 0;		/* and init the two semop() structures */
	postop.sem_op  = 1;
	postop.sem_flg = SEM_UNDO;
	waitop.sem_num = 0;
	waitop.sem_op  = -1;
	waitop.sem_flg = SEM_UNDO;

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
		/* 4start the timer and release the semaphore */
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	if (semop(shared.semid, &postop, 1) == -1) {		/* up by 1 */
		perror("semop error");
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
	if (semctl(shared.semid, 0, IPC_RMID) == -1) {
		perror("semctl error");
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
		if (semop(shared.semid, &waitop, 1) == -1) {
			perror("semop error");
			exit(1);
		}
		shared.counter++;
		if (semop(shared.semid, &postop, 1) == -1) {
			perror("semop error");
			exit(1);
		}
	}
	return(NULL);
}
/* end incr */
