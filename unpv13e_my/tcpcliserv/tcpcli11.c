/* Use standard echo server; baseline measurements for nonblocking version */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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

#define SERV_PORT        9877           /* TCP and UDP */

void
str_cli(FILE *fp, int sockfd);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;
	int ret;

	if (argc != 2) {
		fprintf(stderr, "usage: tcpcli <IPaddress>\n");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(7);
	if ((ret = inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) < 0) {
		fprintf(stderr, "inet_pton error for %s: %s\n",
			argv[1], strerror(errno));	/* errno set */
		exit(1);
	} else if (ret == 0) {
		fprintf(stderr, "inet_pton error for %s\n", argv[1]);	/* errno not set */
		exit(1);
	}

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("connect error");
		exit(1);
	}

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}

