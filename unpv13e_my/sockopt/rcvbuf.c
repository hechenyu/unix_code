#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>		/* for TCP_MAXSEG */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

int
main(int argc, char **argv)
{
	int					sockfd, rcvbuf, mss;
	socklen_t			len;
	struct sockaddr_in	servaddr;

	if (argc != 2) {
		fprintf(stderr, "usage: rcvbuf <IPaddress>\n");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("UDP socket error");
		exit(1);
	}

	len = sizeof(rcvbuf);
	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &len) < 0) {
		perror("getsockopt error");
		exit(1);
	}
	len = sizeof(mss);
	if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len) < 0) {
		perror("getsockopt error");
		exit(1);
	}
	printf("defaults: SO_RCVBUF = %d, MSS = %d\n", rcvbuf, mss);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);		/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		perror("inet_pton error");
		exit(1);
	}

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("connect error");
		exit(1);
	}

	len = sizeof(rcvbuf);
	if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, &len) < 0) {
		perror("getsockopt error");
		exit(1);
	}
	len = sizeof(mss);
	if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &len) < 0) {
		perror("getsockopt error");
		exit(1);
	}
	printf("after connect: SO_RCVBUF = %d, MSS = %d\n", rcvbuf, mss);

	exit(0);
}
