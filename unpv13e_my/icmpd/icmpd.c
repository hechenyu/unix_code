/* include icmpd1 */
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "icmpd.h"

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
 *    kernels still #define it as 5, while actually supporting many more */
#define LISTENQ     1024    /* 2nd argument to listen() */

#define max(a, b) ((a) < (b) ? (b) : (a))

int
main(int argc, char **argv)
{
	int		i, sockfd;
	struct sockaddr_un sun;

	if (argc != 1) {
		fprintf(stderr, "usage: icmpd\n");
		exit(1);
	}

	maxi = -1;					/* index into client[] array */
	for (i = 0; i < FD_SETSIZE; i++)
		client[i].connfd = -1;	/* -1 indicates available entry */
	FD_ZERO(&allset);

	if ((fd4 = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		perror("socket error");
		exit(1);
	}
	FD_SET(fd4, &allset);
	maxfd = fd4;

#ifdef	AF_INET6
	if ((fd6 = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6)) < 0) {
		perror("socket error");
		exit(1);
	}
	FD_SET(fd6, &allset);
	maxfd = max(maxfd, fd6);
#endif

	if ((listenfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}
	sun.sun_family = AF_LOCAL;
	strcpy(sun.sun_path, ICMPD_PATH);
	unlink(ICMPD_PATH);
	if (bind(listenfd, (SA *)&sun, sizeof(sun)) < 0) {
		perror("bind error");
		exit(1);
	}
	if (listen(listenfd, LISTENQ) < 0) {
		perror("listen error");
		exit(1);
	}
	FD_SET(listenfd, &allset);
	maxfd = max(maxfd, listenfd);
/* end icmpd1 */

/* include icmpd2 */
	for ( ; ; ) {
		rset = allset;
		if ((nready = select(maxfd+1, &rset, NULL, NULL, NULL)) < 0) {
			perror("select error");
			exit(1);
		}

		if (FD_ISSET(listenfd, &rset))
			if (readable_listen() <= 0)
				continue;

		if (FD_ISSET(fd4, &rset))
			if (readable_v4() <= 0)
				continue;

#ifdef	AF_INET6
		if (FD_ISSET(fd6, &rset))
			if (readable_v6() <= 0)
				continue;
#endif

		for (i = 0; i <= maxi; i++) {	/* check all clients for data */
			if ( (sockfd = client[i].connfd) < 0)
				continue;
			if (FD_ISSET(sockfd, &rset))
				if (readable_conn(i) <= 0)
					break;				/* no more readable descriptors */
		}
	}
	exit(0);
}
/* end icmpd2 */
