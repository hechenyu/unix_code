#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lock.h"

#define	MAXLINE	4096
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	PATH_PIDFILE	"pidfile"

int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

int
main(int argc, char **argv)
{
	int		pidfd;
	char	line[MAXLINE];

		/* 4open the PID file, create if nonexistent */
	if ((pidfd = open(PATH_PIDFILE, O_RDWR | O_CREAT, FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", PATH_PIDFILE, strerror(errno));
		exit(1);
	}

		/* 4try to write lock the entire file */
	if (write_lock(pidfd, 0, SEEK_SET, 0) < 0) {
		if (errno == EACCES || errno == EAGAIN) {
			fprintf(stderr, "unable to lock %s, is %s already running?",
					 PATH_PIDFILE, argv[0]);
			exit(1);
		} else {
			fprintf(stderr, "unable to lock %s: %s\n", PATH_PIDFILE, strerror(errno));
			exit(1);
		}
	}

		/* 4write my PID, leave file open to hold the write lock */
	snprintf(line, sizeof(line), "%ld\n", (long) getpid());
	if (ftruncate(pidfd, 0) == -1) {
		perror("ftruncate error");
		exit(1);
	}
    if (write(pidfd, line, strlen(line)) != strlen(line)) {
		perror("write error");
		exit(1);
	}

	/* then do whatever the daemon does ... */

	pause();

	return 0;
}
