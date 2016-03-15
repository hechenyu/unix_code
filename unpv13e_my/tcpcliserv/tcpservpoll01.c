/* include fig01 */
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>		/* for OPEN_MAX */

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef HAVE_BZERO
#define bzero(ptr,n)        memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
 *    kernels still #define it as 5, while actually supporting many more */
#define LISTENQ     1024    /* 2nd argument to listen() */

#define SERV_PORT        9877           /* TCP and UDP */

#ifndef	OPEN_MAX
#define	OPEN_MAX	1024
#endif

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

ssize_t
writen(int fd, const void *vptr, size_t n);

int
main(int argc, char **argv)
{
	int					i, maxi, listenfd, connfd, sockfd;
	int					nready;
	ssize_t				n;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct pollfd		client[OPEN_MAX];
	struct sockaddr_in	cliaddr, servaddr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("bind error");
		exit(1);
	}

	if (listen(listenfd, LISTENQ) < 0) {
		perror("listen error");
		exit(1);
	}

	client[0].fd = listenfd;
	client[0].events = POLLRDNORM;
	for (i = 1; i < OPEN_MAX; i++)
		client[i].fd = -1;		/* -1 indicates available entry */
	maxi = 0;					/* max index into client[] array */
/* end fig01 */

/* include fig02 */
	for ( ; ; ) {
		if ((nready = poll(client, maxi+1, INFTIM)) < 0) {
			perror("poll error");
			exit(1);
		}

		if (client[0].revents & POLLRDNORM) {	/* new client connection */
			clilen = sizeof(cliaddr);
			if ((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
				perror("accept error");
				exit(1);
			}

			for (i = 1; i < OPEN_MAX; i++)
				if (client[i].fd < 0) {
					client[i].fd = connfd;	/* save descriptor */
					break;
				}
			if (i == OPEN_MAX) {
				fprintf(stderr, "too many clients\n");
				exit(1);
			}

			client[i].events = POLLRDNORM;
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 1; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i].fd) < 0)
				continue;
			if (client[i].revents & (POLLRDNORM | POLLERR)) {
				if ( (n = read(sockfd, buf, MAXLINE)) < 0) {
					if (errno == ECONNRESET) {
							/*4connection reset by client */
						if (close(sockfd) == -1) {
							perror("close error");
							exit(1);
						}
						client[i].fd = -1;
					} else {
						perror("read error");
						exit(1);
					}
				} else if (n == 0) {
						/*4connection closed by client */
					if (close(sockfd) == -1) {
						perror("close error");
						exit(1);
					}
					client[i].fd = -1;
				} else {
					if (writen(sockfd, buf, n) != n) {
						perror("writen error");
						exit(1);
					}
				}

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
/* end fig02 */
