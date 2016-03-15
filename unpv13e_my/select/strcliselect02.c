#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAXLINE     4096    /* max text line length */

#define max(a,b)    ((a) > (b) ? (a) : (b))

ssize_t
writen(int fd, const void *vptr, size_t n);

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1, stdineof;
	fd_set		rset;
	char		buf[MAXLINE];
	int		n;

	stdineof = 0;
	FD_ZERO(&rset);
	for ( ; ; ) {
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		if (select(maxfdp1, &rset, NULL, NULL, NULL) < 0) {
			perror("select error");
			exit(1);
		}

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if ((n = read(sockfd, buf, MAXLINE)) == -1) {
				perror("read error");
				exit(1);
			} else if (n == 0) {
				if (stdineof == 1) {
					return;		/* normal termination */
				} else {
					fprintf(stderr, "str_cli: server terminated prematurely\n");
					exit(1);
				}
			}

			if (write(fileno(stdout), buf, n) != n) {
				perror("write error");
				exit(1);
			}
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if ((n = read(fileno(fp), buf, MAXLINE)) == -1) {
				perror("read error");
				exit(1);
			} else if (n == 0) {
				stdineof = 1;
				if (shutdown(sockfd, SHUT_WR) < 0) {	/* send FIN */
					perror("shutdown error");
					exit(1);
				}
				FD_CLR(fileno(fp), &rset);
				continue;
			}

			if (writen(sockfd, buf, n) != n) {
				perror("writen error");
				exit(1);
			}
		}
	}
}
