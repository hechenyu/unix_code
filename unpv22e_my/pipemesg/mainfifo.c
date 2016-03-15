#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	FIFO1	"/tmp/fifo.1"
#define	FIFO2	"/tmp/fifo.2"

void client(int, int), server(int, int);

int main(int argc, char **argv)
{
	int readfd, writefd;
	pid_t childpid;

	/* 4Create two FIFOs; OK if they already exist */
	if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST)) {
		fprintf(stderr, "can't create %s: %s\n", FIFO1, strerror(errno));
		exit(1);
	}
	if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST)) {
		unlink(FIFO1);
		fprintf(stderr, "can't create %s: %s\n", FIFO2, strerror(errno));
		exit(1);
	}

	childpid = fork();
	if (childpid == -1) {
		perror("fork error");
		exit(1);
	}
	if (childpid > 0) {		/* parent */
		if ((writefd = open(FIFO1, O_WRONLY, 0)) == -1) {
			fprintf(stderr, "open error for %s: %s\n", FIFO1, strerror(errno));
			exit(1);
		}
		if ((readfd = open(FIFO2, O_RDONLY, 0)) == -1) {
			fprintf(stderr, "open error for %s: %s\n", FIFO2, strerror(errno));
			exit(1);
		}
	
		client(readfd, writefd);
	
		/* wait for child to terminate */
		if (waitpid(childpid, NULL, 0) == -1) {
			perror("waitpid error");
			exit(1);
		}
	
		if (close(readfd) == -1) {
			perror("close error");
			exit(1);
		}
		if (close(writefd) == -1) {
			perror("close error");
			exit(1);
		}
	
		if (unlink(FIFO1) == -1) {
			fprintf(stderr, "unlink error for %s: %s\n", FIFO1, strerror(errno));
			exit(1);
		}
		if (unlink(FIFO2) == -1) {
			fprintf(stderr, "unlink error for %s: %s\n", FIFO2, strerror(errno));
			exit(1);
		}
		exit(0);
	}
	/* 4child */
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
