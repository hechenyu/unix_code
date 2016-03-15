/* Verify that two readers can lock the same region */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lock.h"

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int
lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);

char *
gf_time(void);

int
main(int argc, char **argv)
{
	int		fd;
	pid_t childpid;

	if ((fd = open("test1.data", O_RDWR | O_CREAT, (mode_t) FILE_MODE)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", "test1.data", strerror(errno));
		exit(1);
	}

	if (read_lock(fd, 0, SEEK_SET, 0) == -1) {		/* parent read locks entire file */
		perror("read_lock error");
		exit(1);
	}
	printf("%s: parent has read lock\n", gf_time());

	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	}
	if (childpid == 0) {
			/* 4child */
		if (read_lock(fd, 0, SEEK_SET, 0) == -1) {	/* this should work */
			perror("read_lock error");
			exit(1);
		}
		printf("%s: child has read lock\n", gf_time());
		sleep(2);
		if (un_lock(fd, 0, SEEK_SET, 0) == -1) {
			perror("un_lock error");
			exit(1);
		}
		printf("%s: child releases read lock\n", gf_time());

		if (read_lock(fd, 0, SEEK_SET, 0) == -1) {	/* this should work */
			perror("read_lock error");
			exit(1);
		}
		printf("%s: child has read lock\n", gf_time());
		sleep(2);
		exit(0);
	}

	/* parent */
	sleep(4);
	if (un_lock(fd, 0, SEEK_SET, 0) == -1) {
		perror("un_lock error");
		exit(1);
	}
	printf("%s: parent releases read lock\n", gf_time());
	exit(0);
}
