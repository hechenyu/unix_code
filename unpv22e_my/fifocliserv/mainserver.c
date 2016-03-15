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

ssize_t readline(int fd, void *vptr, size_t maxlen);

void server(int, int);

int main(int argc, char **argv)
{
	int readfifo, writefifo, dummyfd, fd;
	char *ptr, buff[MAXLINE], fifoname[MAXLINE];
	pid_t pid;
	ssize_t	n;

	/* 4create server's well-known FIFO; OK if already exists */
	if ((mkfifo(SERV_FIFO, FILE_MODE) < 0) && (errno != EEXIST)) {
		fprintf(stderr, "can't create %s: %s\n", SERV_FIFO, strerror(errno));
		exit(1);
	}

	/* 4open server's well-known FIFO for reading and writing */
	if ((readfifo = open(SERV_FIFO, O_RDONLY, 0)) == -1) {
		fprintf(stderr, "open error for %s: %s\n", SERV_FIFO, strerror(errno));
		exit(1);
	}
	if ((dummyfd = open(SERV_FIFO, O_WRONLY, 0)) == -1) {		/* never used */
		fprintf(stderr, "open error for %s: %s\n", SERV_FIFO, strerror(errno));
		exit(1);
	}

	while (1) {
		if ((n = readline(readfifo, buff, MAXLINE)) < 0) {
			perror("readline error");
			exit(1);
		}
		if (n == 0)
			break;
		if (buff[n-1] == '\n')
			n--;			/* delete newline from readline() */
		buff[n] = '\0';		/* null terminate pathname */

		if ((ptr = strchr(buff, ' ')) == NULL) {
			fprintf(stderr, "bogus request: %s\n", buff);
			continue;
		}

		*ptr++ = 0;			/* null terminate PID, ptr = pathname */
		pid = atol(buff);
		snprintf(fifoname, sizeof(fifoname), "/tmp/fifo.%ld", (long) pid);
		if ((writefifo = open(fifoname, O_WRONLY, 0)) < 0) {
			fprintf(stderr, "cannot open: %s: %s\n", fifoname, strerror(errno));
			continue;
		}

		if ((fd = open(ptr, O_RDONLY)) < 0) {
			/* 4error: must tell client */
			snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n",
					 strerror(errno));
			n = strlen(ptr);
			if (write(writefifo, ptr, n) != n) {
				perror("write error");
				exit(1);
			}
			if (close(writefifo) == -1) {
				perror("close error");
				exit(1);
			}
	
		} else {
			/* 4open succeeded: copy file to FIFO */
			while (1) {
				if ((n = read(fd, buff, MAXLINE)) < 0) {
					perror("read error");
					exit(1);
				}
				if (n == 0)
					break;
				if (write(writefifo, buff, n) != n) {
					perror("write error");
					exit(1);
				}
			}
			if (close(fd) == -1) {
				perror("close error");
				exit(1);
			}
			if (close(writefifo) == -1) {
				perror("close error");
				exit(1);
			}
		}
	}
	exit(0);
}
