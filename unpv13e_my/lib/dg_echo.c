#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int			n;
	socklen_t	len;
	char		mesg[MAXLINE];

	for ( ; ; ) {
		len = clilen;
		if ((n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len)) < 0) {
			perror("recvfrom error");
			exit(1);
		}

		if (sendto(sockfd, mesg, n, 0, pcliaddr, len) != n) {
			perror("sendto error");
			exit(1);
		}
	}
}
