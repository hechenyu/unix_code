#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAXLINE     4096    /* max text line length */

ssize_t
readline(int fd, void *vptr, size_t maxlen);

ssize_t
writen(int fd, const void *vptr, size_t n);

void
str_echo(int sockfd)
{
	long		arg1, arg2;
	ssize_t		n;
	char		line[MAXLINE];

	for ( ; ; ) {
		if ((n = readline(sockfd, line, MAXLINE)) < 0) {
			perror("readline error");
			exit(1);
		} else if (n == 0) {
			return;		/* connection closed by other end */
		}

		if (sscanf(line, "%ld%ld", &arg1, &arg2) == 2)
			snprintf(line, sizeof(line), "%ld\n", arg1 + arg2);
		else
			snprintf(line, sizeof(line), "input error\n");

		n = strlen(line);
		if (writen(sockfd, line, n) != n) {
			perror("writen error");
			exit(1);
		}
	}
}
