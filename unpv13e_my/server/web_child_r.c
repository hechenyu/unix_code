#include <stdio.h>
#include <stdlib.h>
#include "readline_r.h"

#define	MAXN	16384		/* max #bytes that a client can request */

void
web_child(int sockfd)
{
	int			ntowrite;
	ssize_t		nread;
	char		line[MAXLINE], result[MAXN];
	Rline		rline;

	readline_rinit(sockfd, line, MAXLINE, &rline);
	for ( ; ; ) {
		if ( (nread = readline_r(&rline)) < 0) {
			perror("readline_r error");
			exit(1);
		} else if (nread == 0) {
			return;		/* connection closed by other end */
		}

			/* line from client specifies #bytes to write back */
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
