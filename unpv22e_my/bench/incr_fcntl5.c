#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "lock.h"

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	MAXNPROC	100

int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

inline int min(int a, int b)
{
	return (a < b) ? a : b;
}

int		nloop;

struct shared {
  int	fd;
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
	char	*pathname;
	pid_t	childpid[MAXNPROC];
	int err;
	double st;	// stop time

	if (argc != 4) {
		fprintf(stderr, "usage: incr_fcntl5 <pathname> <#loops> <#processes>\n");
		exit(1);
	}
	pathname = argv[1];
	nloop = atoi(argv[2]);
	nprocs = min(atoi(argv[3]), MAXNPROC);

		/* 4get shared memory for parent and children */
	if ((shared = my_shm(sizeof(struct shared))) == MAP_FAILED) {
		perror("my_shm error");
		exit(1);
	}

		/* 4create the lock file and obtain a write lock */
	if ((shared->fd = open(pathname, O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", pathname, strerror(errno));
		exit(1);
	}
	if (writew_lock(shared->fd, 0, SEEK_SET, 0) == -1) {
		perror("writew_lock error");
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
		/* 4parent: start the timer and release the write lock */
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	if (un_lock(shared->fd, 0, SEEK_SET, 0) == -1) {
		perror("un_lock error");
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
		if (writew_lock(shared->fd, 0, SEEK_SET, 0) == -1) {
			perror("writew_lock error");
			exit(1);
		}
		shared->counter++;
		if (un_lock(shared->fd, 0, SEEK_SET, 0) == -1) {
			perror("un_lock error");
			exit(1);
		}
	}
	return(NULL);
}
/* end incr */
