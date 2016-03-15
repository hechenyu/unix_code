#include <sys/socket.h>
#include <unistd.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXLINE     4096    /* max text line length */

int
udp_connect(const char *host, const char *serv);

int
main(int argc, char **argv)
{
	int		sockfd, n;
	char	recvline[MAXLINE + 1];

	if (argc != 3) {
		fprintf(stderr, "usage: daytimeudpcli2 <hostname/IPaddress> <service/port#>");
		exit(1);
	}

	sockfd = udp_connect(argv[1], argv[2]);

	if (write(sockfd, "", 1) != 1) {	/* send 1-byte datagram */
		perror("write error");
		exit(1);
	}

	if ((n = read(sockfd, recvline, MAXLINE)) < 0) {
		perror("read error");
		exit(1);
	}
	assert(n != 0);
	recvline[n] = '\0';	/* null terminate */
	if (fputs(recvline, stdout) == EOF) {
		perror("fputs error");
		exit(1);
	}

	exit(0);
}
