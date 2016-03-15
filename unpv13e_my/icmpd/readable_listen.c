#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "icmpd.h"

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

int
readable_listen(void)
{
	int			i, connfd;
	socklen_t	clilen;

	clilen = sizeof(cliaddr);
	if ((connfd = accept(listenfd, (SA *)&cliaddr, &clilen)) < 0) {
		perror("accept error");
		exit(1);
	}

		/* 4find first available client[] structure */
	for (i = 0; i < FD_SETSIZE; i++)
		if (client[i].connfd < 0) {
			client[i].connfd = connfd;	/* save descriptor */
			break;
		}
	if (i == FD_SETSIZE) {
		close(connfd);		/* can't handle new client, */
		return(--nready);	/* rudely close the new connection */
	}
	printf("new connection, i = %d, connfd = %d\n", i, connfd);

	FD_SET(connfd, &allset);	/* add new descriptor to set */
	if (connfd > maxfd)
		maxfd = connfd;			/* for select() */
	if (i > maxi)
		maxi = i;				/* max index in client[] array */

	return(--nready);
}
