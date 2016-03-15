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
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int				listenfd, connfd;
	socklen_t		len;
	char			buff[MAXLINE];
	time_t			ticks;
	struct sockaddr_storage	cliaddr;

	if (argc != 2) {
		fprintf(stderr, "usage: daytimetcpsrv1 <service or port#>\n");
		exit(1);
	}

	listenfd = tcp_listen(NULL, argv[1], NULL);

	for ( ; ; ) {
		len = sizeof(cliaddr);
		if ((connfd = accept(listenfd, (SA *)&cliaddr, &len)) < 0) {
			perror("accept error");
			exit(1);
		}
		printf("connection from %s\n", sock_ntop((SA *)&cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		if (write(connfd, buff, strlen(buff)) != strlen(buff)) {
			perror("write error");
			exit(1);
		}

		if (close(connfd) == -1) {
			perror("close error");
			exit(1);
		}
	}
}
