#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	LOCKFILE	"/tmp/seqno.lock"

void my_lock(int fd)
{
	int tempfd;

	while ( (tempfd = open(LOCKFILE, O_RDWR|O_CREAT|O_EXCL, FILE_MODE)) < 0) {
		if (errno != EEXIST) {
			printf("open error for lock file: %s\n", strerror(errno));
			exit(1);
		}
		/* someone else has the lock, loop around and try again */
	}
	if (close(tempfd) == -1) {	/* opened the file, we have the lock */
		printf("close error: %s\n", strerror(errno));
		exit(1);
	}
}

void my_unlock(int fd)
{
	if (unlink(LOCKFILE) == -1) {	/* release lock by removing file */
		printf("unlink error for %s: %s\n", LOCKFILE, strerror(errno));
		exit(1);
	}
}
