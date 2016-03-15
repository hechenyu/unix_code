#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new files */

#define	LOCKFILE	"/tmp/seqno.lock"
#define	PREFIX		"/tmp"		/* prefix for temp pathname */

void
my_lock(int fd)
{
	char	*ptr;
	int tempfd;

	ptr = tempnam(PREFIX, NULL);
		/* create/open and then close */
	if ((tempfd = open(ptr, O_CREAT | O_RDWR | O_TRUNC, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", ptr, strerror(errno));
		exit(1);
	}
	if (close(tempfd) == -1) {
		perror("close error");
		exit(1);
	}

	while (link(ptr, LOCKFILE) < 0) {
		if (errno != EEXIST) {
			perror("link error for lock file");
			exit(1);
		}
		/* someone else has the lock, loop around and try again */
	}
	if (unlink(ptr) == -1) {			/* linked the file, we have the lock */
		perror("unlink error");
		exit(1);
	}
	free(ptr);				/* tempnam() calls malloc() */
}

void
my_unlock(int fd)
{
	if (unlink(LOCKFILE) == -1) {		/* release lock by removing file */
		perror("unlink error");
		exit(1);
	}
}
