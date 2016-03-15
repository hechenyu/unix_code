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
heartbeat_serv(int servfd_arg, int nsec_arg, int maxnalarms_arg);

void
str_echo(int sockfd)
{
	ssize_t		n;
	char		line[MAXLINE];

	heartbeat_serv(sockfd, 1, 5);

	for ( ; ; ) {
		if ((n = readline(sockfd, line, MAXLINE)) < 0) {
			perror("readline error");
			exit(1);
		} else if (n == 0) {
			return;		/* connection closed by other end */
		}

		if (writen(sockfd, line, n) != n) {
			perror("writen error");
			exit(1);
		}
	}
}
