#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "web.h"

#define MAXLINE     4096    /* max text line length */

ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n);

int
tcp_connect(const char *host, const char *serv);

void
home_page(const char *host, const char *fname)
{
	int		fd, n;
	char	line[MAXLINE];

	fd = tcp_connect(host, SERV);	/* blocking connect() */

	n = snprintf(line, sizeof(line), GET_CMD, fname);
	if (writen(fd, line, n) != n) {
		perror("writen error");
		exit(1);
	}

	for ( ; ; ) {
		if ((n = read(fd, line, MAXLINE)) < 0) {
			perror("read error");
			exit(1);
		} else if (n == 0) {
			break;		/* server closed connection */
		}

		printf("read %d bytes of home page\n", n);
		/* do whatever with data */
	}
	printf("end-of-file on home page\n");
	if (close(fd) == -1) {
		perror("close error");
		exit(1);
	}
}
