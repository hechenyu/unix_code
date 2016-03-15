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

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

int
main(int argc, char **argv)
{
	int					sockfd;
	socklen_t			len;
	struct sockaddr_in	cliaddr, servaddr;
	int ret;

	if (argc != 2) {
		fprintf(stderr, "usage: udpcli <IPaddress>\n");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
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

	len = sizeof(cliaddr);
	if (getsockname(sockfd, (SA *) &cliaddr, &len) < 0) {
		perror("getsockname error");
		exit(1);
	}
	printf("local address %s\n", sock_ntop((SA *) &cliaddr, len));

	exit(0);
}
