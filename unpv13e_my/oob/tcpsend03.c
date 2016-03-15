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
		fprintf(stderr, "usage: tcpsend03 <host> <port#>\n");
		exit(1);
	}

	sockfd = tcp_connect(argv[1], argv[2]);

	if (write(sockfd, "123", 3) != 3) {
		perror("write error");
		exit(1);
	}
	printf("wrote 3 bytes of normal data\n");
	sleep(1);

	if (send(sockfd, "4", 1, MSG_OOB) != 1) {
		perror("send error");
		exit(1);
	}
	printf("wrote 1 byte of OOB data\n");
	sleep(1);

	if (write(sockfd, "56", 2) != 2) {
		perror("write error");
		exit(1);
	}
	printf("wrote 2 bytes of normal data\n");
	sleep(1);

	if (send(sockfd, "7", 1, MSG_OOB) != 1) {
		perror("send error");
		exit(1);
	}
	printf("wrote 1 byte of OOB data\n");
	sleep(1);

	if (write(sockfd, "89", 2) != 2) {
		perror("write error");
		exit(1);
	}
	printf("wrote 2 bytes of normal data\n");
	sleep(1);

	exit(0);
}
