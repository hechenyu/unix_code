#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE     4096    /* max text line length */

#define	MAXN	16384		/* max # bytes client can request */

ssize_t
readline(int fd, void *vptr, size_t maxlen);

ssize_t
writen(int fd, const void *vptr, size_t n);

void
web_child(int sockfd)
{
	int			ntowrite;
	ssize_t		nread;
	char		line[MAXLINE], result[MAXN];

	for ( ; ; ) {
		if ( (nread = readline(sockfd, line, MAXLINE)) < 0) {
			perror("readline error");
			exit(1);
		} else if (nread == 0) {
			return;		/* connection closed by other end */
		}

			/* 4line from client specifies #bytes to write back */
		ntowrite = atol(line);
		if ((ntowrite <= 0) || (ntowrite > MAXN)) {
			fprintf(stderr, "client request for %d bytes\n", ntowrite);
			exit(1);
		}

		if (writen(sockfd, result, ntowrite) != ntowrite) {
			perror("writen error");
			exit(1);
		}
	}
}
