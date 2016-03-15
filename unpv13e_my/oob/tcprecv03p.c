#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* POSIX requires that an #include of <poll.h> DefinE INFTIM, but many
   systems still DefinE it in <sys/stropts.h>.  We don't want to include
   all the STREAMS stuff if it's not needed, so we just DefinE INFTIM here.
   This is the standard value, but there's no guarantee it is -1. */
#ifndef INFTIM
#define INFTIM          (-1)    /* infinite poll timeout */
/* $$.Ic INFTIM$$ */
#ifdef	HAVE_POLL_H
#define	INFTIM_UNPH				/* tell unpxti.h we defined it */
#endif
#endif

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int		listenfd, connfd, n, justreadoob = 0;
	char	buff[100];
	struct pollfd	pollfd[1];

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], NULL);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], NULL);
	} else {
		fprintf(stderr, "usage: tcprecv03p [ <host> ] <port#>");
		exit(1);
	}

	if ((connfd = accept(listenfd, NULL, NULL)) < 0) {
		perror("accept error");
		exit(1);
	}

	pollfd[0].fd = connfd;
	pollfd[0].events = POLLRDNORM;
	for ( ; ; ) {
		if (justreadoob == 0)
			pollfd[0].events |= POLLRDBAND;

		if (poll(pollfd, 1, INFTIM) < 0) {
			perror("poll error");
			exit(1);
		}

		if (pollfd[0].revents & POLLRDBAND) {
			if ((n = recv(connfd, buff, sizeof(buff)-1, MSG_OOB)) < 0) {
				perror("recv error");
				exit(1);
			}
			buff[n] = 0;		/* null terminate */
			printf("read %d OOB byte: %s\n", n, buff);
			justreadoob = 1;
			pollfd[0].events &= ~POLLRDBAND;	/* turn bit off */
		}

		if (pollfd[0].revents & POLLRDNORM) {
			if ( (n = read(connfd, buff, sizeof(buff)-1)) < 0) {
				perror("read error");
				exit(1);
			} else if (n == 0) {
				printf("received EOF\n");
				exit(0);
			}
			buff[n] = 0;	/* null terminate */
			printf("read %d bytes: %s\n", n, buff);
			justreadoob = 0;
		}
	}
}
