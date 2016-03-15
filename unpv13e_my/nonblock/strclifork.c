#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define MAXLINE     4096    /* max text line length */

ssize_t
readline(int fd, void *vptr, size_t maxlen);

ssize_t
writen(int fd, const void *vptr, size_t n);

void
str_cli(FILE *fp, int sockfd)
{
	pid_t	pid;
	char	sendline[MAXLINE], recvline[MAXLINE];
	int n;

	if ( (pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) {		/* child: server -> stdout */
		while ((n = readline(sockfd, recvline, MAXLINE)) > 0) {
			if (fputs(recvline, stdout) == EOF) {
				perror("fputs error");
				exit(1);
			}
		}
		if (n < 0) {
			perror("readline error");
			exit(1);
		}

		kill(getppid(), SIGTERM);	/* in case parent still running */
		exit(0);
	}

		/* parent: stdin -> server */
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		if (writen(sockfd, sendline, strlen(sendline)) != strlen(sendline)) {
			perror("writen error");
			exit(1);
		}
	}

	if (ferror(fp)) {
		perror("fgets error");
		exit(1);
	}

	if (shutdown(sockfd, SHUT_WR) < 0) {	/* EOF on stdin, send FIN */
		perror("shutdown error");
		exit(1);
	}
	pause();
	return;
}
