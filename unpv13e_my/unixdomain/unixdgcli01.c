#include <sys/socket.h>
#include <sys/un.h>

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

#define UNIXDG_PATH     "/tmp/unix.dg"  /* Unix domain datagram */

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_un	cliaddr, servaddr;

	if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&cliaddr, sizeof(cliaddr));		/* bind an address for us */
	cliaddr.sun_family = AF_LOCAL;
	strcpy(cliaddr.sun_path, tmpnam(NULL));

	if (bind(sockfd, (SA *) &cliaddr, sizeof(cliaddr)) < 0) {
		perror("bind error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));	/* fill in server's address */
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXDG_PATH);

	dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

	exit(0);
}
