/* include fig01 */
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

ssize_t
readline(int fd, void *vptr, size_t maxlen);

ssize_t
writen(int fd, const void *vptr, size_t n);

int
main(int argc, char **argv)
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE];
	ssize_t				n;
	fd_set				rset, allset;
	char				line[MAXLINE];
	socklen_t			clilen;
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

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for ( ; ; ) {
		rset = allset;
		if ((nready = select(maxfd+1, &rset, NULL, NULL, NULL)) < 0) {
			perror("select error");
			exit(1);
		}

		if (FD_ISSET(listenfd, &rset)) {	/* new client connection */
			printf("listening socket readable\n");
			sleep(5);
			clilen = sizeof(cliaddr);
			if ((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
				perror("accept error");
				exit(1);
			}
#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
#endif

			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) {
					client[i] = connfd;	/* save descriptor */
					break;
				}
			if (i == FD_SETSIZE) {
				fprintf(stderr, "too many clients\n");
				exit(1);
			}

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i]) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset)) {
				if ((n = readline(sockfd, line, MAXLINE)) < 0) {
					perror("readline error");
					exit(1);
				} else if (n == 0) {
						 /* connection closed by client */
					if (close(sockfd) == -1) {
						perror("close error");
						exit(1);
					}
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
				if (writen(sockfd, line, n) != n) {
					perror("writen error");
					exit(1);
				}

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
