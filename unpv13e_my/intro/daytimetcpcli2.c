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
main(int argc, char **argv)
{
	int					sockfd, n, counter = 0;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2) {
		fprintf(stderr, "usage: a.out <IPaddress>\n");
		exit(1);
	}

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(9999);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		fprintf(stderr, "inet_pton error for %s\n", argv[1]);
		exit(1);
	}

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("connect error");
		exit(1);
	}

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		counter++;
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF) {
			perror("fputs error");
			exit(1);
		}
	}
	if (n < 0) {
		perror("read error");
		exit(1);
	}

	printf("counter = %d\n", counter);
	exit(0);
}
