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
	int				i, listenfd, connfd;
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;
	char			buff[MAXLINE];
	time_t			ticks;

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: daytimetcpsrv2 [ <host> ] <service or port>\n");
		exit(1);
	}

	if ((cliaddr = malloc(addrlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	for ( ; ; ) {
		len = addrlen;
		if ((connfd = accept(listenfd, cliaddr, &len)) < 0) {
			perror("accept error");
			exit(1);
		}
		printf("connection from %s\n", sock_ntop(cliaddr, len));

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		for (i = 0; i < strlen(buff); i++) {
        	if (send(connfd, &buff[i], 1, MSG_EOR) != 1) {
				perror("send error");
				exit(1);
			}
		}

		if (close(connfd) == -1) {
			perror("close error");
			exit(1);
		}
	}
}
