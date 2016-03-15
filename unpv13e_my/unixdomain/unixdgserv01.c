#include <sys/socket.h>
#include <sys/un.h>
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

#define UNIXDG_PATH     "/tmp/unix.dg"  /* Unix domain datagram */

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_un	servaddr, cliaddr;

	if ((sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	unlink(UNIXDG_PATH);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family      = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXDG_PATH);

	if (bind(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("bind error");
		exit(1);
	}

	dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
	exit(0);
}
