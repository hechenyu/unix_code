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
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;
	struct linger	ling;
	char			buff[MAXLINE];
	time_t			ticks;

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: daytimetcpsrv3 [ <host> ] <service or port>\n");
		exit(1);
	}

	if ((cliaddr = malloc(addrlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	for ( ; ; ) {
		len = addrlen;
		if ((connfd = accept(listenfd, (SA *)cliaddr, &len)) < 0) {
			perror("accept error");
			exit(1);
		}
		printf("connection from %s\n", sock_ntop(cliaddr, len));

			/* force RST instead of FIN after data */
		ling.l_onoff = 1;
		ling.l_linger = 0;
		if (setsockopt(connfd, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) < 0) {
			perror("setsockopt error");
			exit(1);
		}

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		if (write(connfd, buff, strlen(buff)) != strlen(buff)) {
			perror("write error");
			exit(1);
		}

		sleep(2);	/* let data get across before RST */
		if (close(connfd) == -1) {
			perror("close error");
			exit(1);
		}
	}
}
