#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "web.h"

#define MAXLINE     4096    /* max text line length */

struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype, int *n);

void
start_connect(struct file *fptr)
{
	int				fd, flags, n;
	struct addrinfo	*ai;

	if ((ai = host_serv(fptr->f_host, SERV, 0, SOCK_STREAM, &n)) == NULL) {
		fprintf(stderr, "host_serv error for %s, %s: %s",
				 (fptr->f_host == NULL) ? "(no hostname)" : fptr->f_host,
				 (SERV == NULL) ? "(no service name)" : SERV,
				 gai_strerror(n));
		exit(1);
	}

	if ((fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
		perror("socket error");
		exit(1);
	}
	fptr->f_fd = fd;
	printf("start_connect for %s, fd %d\n", fptr->f_name, fd);

		/* 4Set socket nonblocking */
	if ((flags = fcntl(fd, F_GETFL, 0)) == -1) {
		perror("fcntl error");
		exit(1);
	}
	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		perror("fcntl error");
		exit(1);
	}

		/* 4Initiate nonblocking connect to the server. */
	if ( (n = connect(fd, ai->ai_addr, ai->ai_addrlen)) < 0) {
		if (errno != EINPROGRESS) {
			perror("nonblocking connect error");
			exit(1);
		}
		fptr->f_flags = F_CONNECTING;
		FD_SET(fd, &rset);			/* select for reading and writing */
		FD_SET(fd, &wset);
		if (fd > maxfd)
			maxfd = fd;

	} else if (n >= 0)				/* connect is already done */
		write_get_cmd(fptr);	/* write() the GET command */
}
