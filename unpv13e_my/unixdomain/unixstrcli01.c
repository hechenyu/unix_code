#include <sys/socket.h>
#include <sys/un.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef HAVE_BZERO
#define bzero(ptr,n)        memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define UNIXSTR_PATH    "/tmp/unix.str" /* Unix domain stream */

void
str_cli(FILE *fp, int sockfd);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_un	servaddr;

	if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("connect error");
		exit(1);
	}

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}

