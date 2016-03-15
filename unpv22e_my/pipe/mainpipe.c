#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void client(int, int), server(int, int);

int main(int argc, char **argv)
{
	int pipe1[2], pipe2[2];
	pid_t childpid;

	/* create two pipes */
	if (pipe(pipe1) < 0) {
		perror("pipe error");
		exit(1);
	}
	if (pipe(pipe2) < 0) {
		perror("pipe error");
		exit(1);
	}

	childpid = fork();
	if (childpid == -1) {
		perror("fork error");
		exit(1);
	}
	if (childpid == 0) {	/* child */
		if (close(pipe1[1]) == -1) {
			perror("close error");
			exit(1);
		}
		if (close(pipe2[0]) == -1) {
			perror("close error");
			exit(1);
		}

		server(pipe1[0], pipe2[1]);
		exit(0);
	}
	/* 4parent */
	if (close(pipe1[0]) == -1) {
		perror("close error");
		exit(1);
	}
	if (close(pipe2[1]) == -1) {
		perror("close error");
		exit(1);
	}

	client(pipe2[0], pipe1[1]);

	/* wait for child to terminate */
	if (waitpid(childpid, NULL, 0) == -1) {
		perror("waitpid error");
		exit(1);
	}
	exit(0);
}
