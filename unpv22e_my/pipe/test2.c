#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd[2], n;
	pid_t childpid;
	fd_set wset;

	if (pipe(fd) < 0) {
		perror("pipe error");
		exit(1);
	}

	childpid = fork();
	if (childpid == -1) {
		perror("fork error");
		exit(1);
	}
	if (childpid == 0) {		/* child */
		printf("child closing pipe read descriptor\n");
		if (close(fd[0]) == -1) {
			printf("close error: %s\n", strerror(errno));
			exit(1);
		}
		sleep(6);
		exit(0);
	}
	/* 4parent */
	if (close(fd[0]) == -1) {	/* in case of a full-duplex pipe */
		perror("close error");
		exit(1);
	}
	sleep(3);
	FD_ZERO(&wset);
	FD_SET(fd[1], &wset);
	n = select(fd[1] + 1, NULL, &wset, NULL, NULL);
	printf("select returned %d\n", n);

	if (FD_ISSET(fd[1], &wset)) {
		printf("fd[1] writable\n");
		if (write(fd[1], "hello", 5) != 5) {
			printf("write error: %s\n", strerror(errno));
			exit(1);
		}
	}

	exit(0);
}
