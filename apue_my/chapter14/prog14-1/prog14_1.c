/* 程序14-1 经由管道父进程向子进程传送数据 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	MAXLINE	4096

int main(void)
{
	int n, fd[2];
	pid_t pid;
	char line[MAXLINE];

	if (pipe(fd) < 0) {
		perror("pipe error");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid > 0) {	/* parent */
		close(fd[0]);
		write(fd[1], "hello world\n", 12);
	} else {
		close(fd[1]);
		n = read(fd[0], line, MAXLINE);
		write(STDOUT_FILENO, line, n);
	}

	exit(0);
}
