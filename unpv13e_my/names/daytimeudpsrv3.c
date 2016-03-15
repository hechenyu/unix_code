#include <sys/socket.h>
#include <unistd.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

int
udp_server_reuseaddr(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int				sockfd;
	ssize_t			n;
	char			buff[MAXLINE];
	time_t			ticks;
	socklen_t		len;
	struct sockaddr_storage	cliaddr;

	if (argc == 2) {
		sockfd = udp_server_reuseaddr(NULL, argv[1], NULL);
	} else if (argc == 3) {
		sockfd = udp_server_reuseaddr(argv[1], argv[2], NULL);
	} else {
		fprintf(stderr, "usage: daytimeudpsrv [ <host> ] <service or port>\n");
		exit(1);
	}

	for ( ; ; ) {
		len = sizeof(cliaddr);
		if ((n = recvfrom(sockfd, buff, MAXLINE, 0, (SA *)&cliaddr, &len)) < 0) {
			perror("recvfrom error");
			exit(1);
		}
		printf("datagram from %s\n", sock_ntop((SA *)&cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		if (sendto(sockfd, buff, strlen(buff), 0, (SA *)&cliaddr, len) != strlen(buff)) {
			perror("sendto error");
			exit(1);
		}
	}
}
