#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	LOCKFILE	"/tmp/seqno.lock"

void
my_lock(int fd)
{
	int		tempfd;

		/* create with O_TRUNC and no permission bits */
	while ( (tempfd = open(LOCKFILE, O_CREAT | O_WRONLY | O_TRUNC, 0)) < 0) {
		if (errno != EACCES) {
			fprintf(stderr, "open error for lock file %s: %s\n", LOCKFILE, strerror(errno));
			exit(1);
		}
		/* someone else has the lock, loop around and try again */
	}
	if (close(tempfd) == -1) {			/* opened the file, we have the lock */
		perror("close error");
		exit(1);
	}
}

void
my_unlock(int fd)
{
	if (unlink(LOCKFILE) == -1) {		/* release lock by removing file */
		perror("unlink error");
		exit(1);
	}
}
