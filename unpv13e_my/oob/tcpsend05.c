#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

int
tcp_connect(const char *host, const char *serv);

int
main(int argc, char **argv)
{
	int		sockfd, size;
	char	buff[16384];

	if (argc != 3) {
		fprintf(stderr, "usage: tcpsend05 <host> <port#>\n");
		exit(1);
	}

	sockfd = tcp_connect(argv[1], argv[2]);

	size = 32768;
	if (setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	if (write(sockfd, buff, 16384) != 16384) {
		perror("write error");
		exit(1);
	}
	printf("wrote 16384 bytes of normal data\n");
	sleep(5);

	if (send(sockfd, "a", 1, MSG_OOB) != 1) {
		perror("send error");
		exit(1);
	}
	printf("wrote 1 byte of OOB data\n");

	if (write(sockfd, buff, 1024) != 1024) {
		perror("write error");
		exit(1);
	}
	printf("wrote 1024 bytes of normal data\n");

	exit(0);
}
