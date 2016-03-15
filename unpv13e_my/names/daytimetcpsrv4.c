#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE     4096    /* max text line length */

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int				listenfd, connfd;
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;
	char			buff[MAXLINE], host[NI_MAXHOST], serv[NI_MAXSERV];
	time_t			ticks;

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: daytimetcpsrv4 [ <host> ] <service or port>\n");
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
		if (getnameinfo(cliaddr, len, host, NI_MAXHOST, serv, NI_MAXSERV,
						NI_NUMERICHOST | NI_NUMERICSERV) == 0)
			printf("connection from %s.%s\n", host, serv);

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
