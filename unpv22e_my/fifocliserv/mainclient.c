#include "fifo.h"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define	MAXLINE	4096
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char **argv)
{
	int readfifo, writefifo;
	size_t len;
	ssize_t	n;
	char *ptr, fifoname[MAXLINE], buff[MAXLINE];
	pid_t pid;

	/* 4create FIFO with our PID as part of name */
	pid = getpid();
	snprintf(fifoname, sizeof(fifoname), "/tmp/fifo.%ld", (long) pid);
	if ((mkfifo(fifoname, FILE_MODE) < 0) && (errno != EEXIST)) {
		fprintf(stderr, "can't create %s: %s\n", fifoname, strerror(errno));
		exit(1);
	}

	/* 4start buffer with pid and a blank */
	snprintf(buff, sizeof(buff), "%ld ", (long) pid);
	len = strlen(buff);
	ptr = buff + len;

	/* 4read pathname */
	if (fgets(ptr, MAXLINE - len, stdin) == NULL && ferror(stdin)) {
		fprintf(stderr, "fgets error: %s\n", strerror(errno));
		exit(1);
	}
	len = strlen(buff);		/* fgets() guarantees null byte at end */

	/* 4open FIFO to server and write PID and pathname to FIFO */
	if ((writefifo = open(SERV_FIFO, O_WRONLY, 0)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", SERV_FIFO, strerror(errno));
		exit(1);
	}
	if (write(writefifo, buff, len) != len) {
		perror("write error");
		exit(1);
	}

	/* 4now open our FIFO; blocks until server opens for writing */
	if ((readfifo = open(fifoname, O_RDONLY, 0)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", fifoname, strerror(errno));
		exit(1);
	}

	/* 4read from IPC, write to standard output */
	while (1) {
		if ((n = read(readfifo, buff, MAXLINE)) < 0) {
			perror("read error");
			exit(1);
		}
		if (n == 0)
			break;
		if (write(STDOUT_FILENO, buff, n) != n) {
			perror("write error");
			exit(1);
		}
	}

	if (close(readfifo) == -1) {
		perror("close error");
		exit(1);
	}
	if (unlink(fifoname) == -1) {
		fprintf(stderr, "unlink error for %s: %s\n", fifoname, strerror(errno));
		exit(1);
	}
	exit(0);
}
