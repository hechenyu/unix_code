#include <sys/socket.h>
#include <unistd.h>
#include <syslog.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE     4096    /* max text line length */

int
daemon_init(const char *pname, int facility);

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t addrlen, len;
	struct sockaddr	*cliaddr;
	char buff[MAXLINE];
	time_t ticks;

	if (argc < 2 || argc > 3) {
		fprintf(stderr, "usage: daytimetcpsrv2 [ <host> ] <service or port>\n");
		exit(1);
	}

	daemon_init(argv[0], 0);

	if (argc == 2)
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	else
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);

	if ((cliaddr = malloc(addrlen)) == NULL) {
		syslog(LOG_ERR, "malloc error: %m");
		exit(1);
	}

	for ( ; ; ) {
		len = addrlen;
		if ((connfd = accept(listenfd, cliaddr, &len)) < 0) {
			syslog(LOG_ERR, "accept error: %m");
			exit(1);
		}
		syslog(LOG_INFO, "connection from %s\n", sock_ntop(cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		if (write(connfd, buff, strlen(buff)) != strlen(buff)) {
			syslog(LOG_ERR, "write error: %m");
			exit(1);
		}

		if (close(connfd) < 0) {
			syslog(LOG_ERR, "close error: %m");
			exit(1);
		}
	}
}
