#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAXLINE     4096    /* max text line length */

ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n);

void
str_echo(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0) {
		if (writen(sockfd, buf, n) != n) {
			perror("writen error");
			exit(1);
		}
	}

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0) {
		perror("str_echo: read error");
		exit(1);
	}
}
