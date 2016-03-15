/* 程序14-9 驱动add2过滤程序的程序 */
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	MAXLINE	4096

static void sig_pipe(int);		/* our signal handler */
int s_pipe(int fd[2]);

int main(void)
{
	int n, fd[2];
	pid_t pid;
	char line[MAXLINE];

	if (signal(SIGPIPE, sig_pipe) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	if (s_pipe(fd) < 0) {
		perror("pipe error");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid > 0) {		/* parent */
		close(fd[1]);
		while (fgets(line, MAXLINE, stdin) != NULL) {
			n = strlen(line);
			if (write(fd[0], line, n) != n) {
				perror("write error to pipe");
				exit(1);
			}
			if ((n = read(fd[0], line, MAXLINE)) < 0) {
				perror("read error from pipe");
				exit(1);
			}
			if (n == 0) {
				perror("child closed pipe");
				break;
			}
			line[n] = 0;		/* null terminate */
			if (fputs(line, stdout) == EOF) {
				perror("fputs error");
				exit(1);
			}
		}
		if (ferror(stdin)) {
			perror("fgets error on stdin");
			exit(1);
		}
		exit(0);
	} else {			/* child */
		close(fd[0]);
		if (fd[1] != STDIN_FILENO) {
			if (dup2(fd[1], STDIN_FILENO) != STDIN_FILENO) {
				perror("dup2 error to stdin");
				exit(1);
			}
		}
		if (fd[1] != STDOUT_FILENO) {
			if (dup2(fd[1], STDOUT_FILENO) != STDOUT_FILENO) {
				perror("dup2 error to stdout");
				exit(1);
			}
		}
		if (execl("./add2", "add2", (char *) 0) < 0) {
			perror("execl error");
			exit(1);
		}
	}
}

/*
int s_pipe(int fd[2])
{
	return(pipe(fd));
}
*/

int s_pipe(int fd[2])       /* two file descriptors returned in fd[0] & fd[1] */
{
	return(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}

static void sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}

