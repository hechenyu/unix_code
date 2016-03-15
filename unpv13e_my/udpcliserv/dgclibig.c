#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#undef	MAXLINE
#define	MAXLINE 65507

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int			size;
	char		sendline[MAXLINE], recvline[MAXLINE + 1];
	ssize_t		n;

	size = 70000;
	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) < 0) {
		perror("setsockopt error");
		exit(1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	if (sendto(sockfd, sendline, MAXLINE, 0, pservaddr, servlen) != MAXLINE) {
		perror("sendto error");
		exit(1);
	}

	if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0) {
		perror("recvfrom error");
		exit(1);
	}

	printf("received %d bytes\n", n);
}
