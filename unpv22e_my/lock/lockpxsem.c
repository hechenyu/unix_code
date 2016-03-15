#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	LOCK_PATH	"/pxsemlock"

sem_t	*locksem;
int		initflag;

void
my_lock(int fd)
{
	if (initflag == 0) {
		if ((locksem = sem_open(LOCK_PATH, O_CREAT, FILE_MODE, 1)) == SEM_FAILED) {
			fprintf(stderr, "sem_open error for %s: %s\n", LOCK_PATH, strerror(errno));
			exit(1);
		}
		initflag = 1;
	}
	if (sem_wait(locksem) == -1) {
		perror("sem_wait error");
		exit(1);
	}
}

void
my_unlock(int fd)
{
	if (sem_post(locksem) == -1) {
		perror("sem_post error");
		exit(1);
	}
}
