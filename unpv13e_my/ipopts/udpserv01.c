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

#define SERV_PORT        9877           /* TCP and UDP */

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in6	servaddr, cliaddr;

	if ((sockfd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin6_family     = AF_INET6;
	servaddr.sin6_addr       = in6addr_any;
	servaddr.sin6_port       = htons(SERV_PORT);

	if (bind(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("bind error");
		exit(1);
	}

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}
