#include <sys/socket.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

int
tcp_connect(const char *host, const char *serv);

char *
sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

int
main(int argc, char **argv)
{
	int				sockfd, n;
	char			recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr_storage	ss;

	if (argc != 3) {
		fprintf(stderr,
			"usage: daytimetcpcli <hostname/IPaddress> <service/port#>\n");
		exit(1);
	}

	sockfd = tcp_connect(argv[1], argv[2]);

	len = sizeof(ss);
	if (getpeername(sockfd, (SA *)&ss, &len) < 0) {
		perror("getpeername error");
		exit(1);
	}
	printf("connected to %s\n", sock_ntop_host((SA *)&ss, len));

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
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
	exit(0);
}
