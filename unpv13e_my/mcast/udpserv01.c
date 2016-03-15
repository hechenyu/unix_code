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

int
mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex);

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr, grpaddr, cliaddr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(9999);

	if (bind(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("bind error");
		exit(1);
	}

	bzero(&grpaddr, sizeof(servaddr));
	grpaddr.sin_family      = AF_INET;
	grpaddr.sin_addr.s_addr = inet_addr("224.0.0.1");

	mcast_join(sockfd, &grpaddr, sizeof(grpaddr), NULL, 0);

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
	exit(0);
}
