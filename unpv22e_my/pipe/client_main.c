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

void client(int, int);

int main(int argc, char **argv)
{
	int readfd, writefd;

	if ((writefd = open(FIFO1, O_WRONLY, 0)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", FIFO1, strerror(errno));
		exit(1);
	}
	if ((readfd = open(FIFO2, O_RDONLY, 0)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", FIFO2, strerror(errno));
		exit(1);
	}

	client(readfd, writefd);

	if (close(readfd) == -1) {
		perror("close error");
		exit(1);
	}
	if (close(writefd) == -1) {
		perror("close error");
		exit(1);
	}

	if (unlink(FIFO1) == -1) {
		printf("unlink error for %s: %s\n", FIFO1, strerror(errno));
		exit(1);
	}
	if (unlink(FIFO2) == -1) {
		printf("unlink error for %s: %s\n", FIFO2, strerror(errno));
		exit(1);
	}
	exit(0);
}
