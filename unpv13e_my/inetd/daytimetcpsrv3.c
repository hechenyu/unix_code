#include <sys/socket.h>
#include <unistd.h>
#include <syslog.h>

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINE     4096    /* max text line length */

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

int
main(int argc, char **argv)
{
	socklen_t		len;
	struct sockaddr	*cliaddr;
	char			buff[MAXLINE];
	time_t			ticks;

	openlog(argv[0], LOG_PID, 0);

	if ((cliaddr = malloc(sizeof(struct sockaddr_storage))) == NULL) {
		syslog(LOG_ERR, "malloc error: %m");
		exit(1);
	}
	len = sizeof(struct sockaddr_storage);
	if (getpeername(0, cliaddr, &len) < 0) {
		syslog(LOG_ERR, "getpeername error: %m");
		exit(1);
	}
	syslog(LOG_INFO, "connection from %s", sock_ntop(cliaddr, len));

    ticks = time(NULL);
    snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	if (write(0, buff, strlen(buff)) != strlen(buff)) {
		syslog(LOG_ERR, "write error: %m");
		exit(1);
	}

	if (close(0) < 0) {	/* close TCP connection */
		syslog(LOG_ERR, "close error: %m");
		exit(1);
	}
	exit(0);
}
