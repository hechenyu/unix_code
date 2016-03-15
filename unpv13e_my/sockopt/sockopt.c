#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/tcp.h>		/* for TCP_MAXSEG */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	int			sockfd, mss, sendbuff;
	socklen_t	optlen;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

		/* Fetch and print the TCP maximum segment size.  */
	optlen = sizeof(mss);
	if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &mss, &optlen) < 0) {
		perror("getsockopt error");
		exit(1);
	}
	printf("TCP mss = %d\n", mss);

		/* Set the send buffer size, then fetch it and print its value.  */
	sendbuff = 65536;
	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuff, sizeof(sendbuff)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	optlen = sizeof(sendbuff);
	if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sendbuff, &optlen) < 0) {
		perror("getsockopt error");
		exit(1);
	}
	printf("send buffer size = %d\n", sendbuff);
	exit(0);
}
