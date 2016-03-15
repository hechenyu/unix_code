#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd[2], n;
	char c;
	pid_t childpid;

	/* assumes a full-duplex pipe (e.g., SVR4) */
	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, fd) < 0) {
		perror("socketpair error");
		exit(1);
	}

	childpid = fork();
	if (childpid == -1) {
		perror("fork error");
		exit(1);
	}
	if (childpid == 0) {		/* child */
		if (close(fd[1]) < 0) {
			perror("close error");
			exit(1);
		}
		sleep(3);
		if ((n = read(fd[0], &c, 1)) < 0) {
			perror("read error");
			exit(1);
		}
		if (n != 1) {
			fprintf(stderr, "child: read returned %d\n", n);
			exit(1);
		}
		printf("child read %c\n", c);
		if (write(fd[0], "c", 1) != 1) {
			perror("write error");
			exit(1);
		}
		exit(0);
	}
	/* 4parent */
	if (close(fd[0]) < 0) {
		perror("close error");
		exit(1);
	}
	if (write(fd[1], "p", 1) != 1) {
		perror("write error");
		exit(1);
	}
	if ((n = read(fd[1], &c, 1)) < 0) {
		perror("read error");
		exit(1);
	}
	if (n != 1) {
		fprintf(stderr, "parent: read returned %d\n", n);
		exit(1);
	}
	printf("parent read %c\n", c);
	exit(0);
}

