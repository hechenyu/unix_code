/* 程序14-9 驱动add2过滤程序的程序 */
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	MAXLINE	4096

static void sig_pipe(int);		/* our signal handler */

int main(void)
{
	int n, fd1[2], fd2[2];
	pid_t pid;
	char line[MAXLINE];

	if (signal(SIGPIPE, sig_pipe) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	if (pipe(fd1) < 0 || pipe(fd2) < 0) {
		perror("pipe error");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid > 0) {		/* parent */
		close(fd1[0]);
		close(fd2[1]);
		while (fgets(line, MAXLINE, stdin) != NULL) {
			n = strlen(line);
			if (write(fd1[1], line, n) != n) {
				perror("write error to pipe");
				exit(1);
			}
			if ((n = read(fd2[0], line, MAXLINE)) < 0) {
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
		close(fd1[1]);
		close(fd2[0]);
		if (fd1[0] != STDIN_FILENO) {
			if (dup2(fd1[0], STDIN_FILENO) != STDIN_FILENO) {
				perror("dup2 error to stdin");
				exit(1);
			}
			close(fd1[0]);
		}
		if (fd2[1] != STDOUT_FILENO) {
			if (dup2(fd2[1], STDOUT_FILENO) != STDOUT_FILENO) {
				perror("dup2 error to stdout");
				exit(1);
			}
			close(fd2[1]);
		}
		if (execl("./add2", "add2", (char *) 0) < 0) {
			perror("execl error");
			exit(1);
		}
	}
}

static void sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	exit(1);
}

