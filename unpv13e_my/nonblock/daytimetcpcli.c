#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef HAVE_BZERO
#define bzero(ptr,n)        memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

int
connect_nonb(int sockfd, const SA *saptr, socklen_t salen, int nsec);

int
main(int argc, char **argv)
{
	int					sockfd, n;
	struct sockaddr_in	servaddr;
	char				recvline[MAXLINE + 1];

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	servaddr.sin_port        = htons(13);	/* daytime server */

	if (connect_nonb(sockfd, (SA *) &servaddr, sizeof(servaddr), 0) < 0) {
		perror("connect_nonb error");
		exit(1);
	}

	for ( ; ; ) {
		if ( (n = read(sockfd, recvline, MAXLINE)) <= 0) {
			if (n == 0)
				break;		/* server closed connection */
			else {
				perror("read error");
				exit(1);
			}
		}
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF) {
			perror("fputs error");
			exit(1);
		}
	}
	exit(0);
}
