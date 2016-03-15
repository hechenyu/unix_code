#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int
tcp_connect(const char *host, const char *serv);

int
main(int argc, char **argv)
{
	int		sockfd;

	if (argc != 3) {
		fprintf(stderr, "usage: tcpsend04 <host> <port#>\n");
		exit(1);
	}

	sockfd = tcp_connect(argv[1], argv[2]);

	if (write(sockfd, "123", 3) != 3) {
		perror("write error");
		exit(1);
	}
	printf("wrote 3 bytes of normal data\n");

	if (send(sockfd, "4", 1, MSG_OOB) != 1) {
		perror("send error");
		exit(1);
	}
	printf("wrote 1 byte of OOB data\n");

	if (write(sockfd, "5", 1) != 1) {
		perror("write error");
		exit(1);
	}
	printf("wrote 1 bytes of normal data\n");

	exit(0);
}
