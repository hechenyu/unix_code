#include "fifo.h"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void server(int, int);

int main(int argc, char **argv)
{
	int readfd, writefd;

	/* 4create two FIFOs; OK if they already exist */
	if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST)) {
		fprintf(stderr, "can't create %s: %s\n", FIFO1, strerror(errno));
		exit(1);
	}
	if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST)) {
		unlink(FIFO1);
		fprintf(stderr, "can't create %s: %s\n", FIFO2, strerror(errno));
		exit(1);
	}

	if ((readfd = open(FIFO1, O_RDONLY, 0)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", FIFO1, strerror(errno));
		exit(1);
	}
	if ((writefd = open(FIFO2, O_WRONLY, 0)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", FIFO2, strerror(errno));
		exit(1);
	}

	server(readfd, writefd);
	exit(0);
}
