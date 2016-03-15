/* include udp_server */
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef HAVE_BZERO
#define bzero(ptr,n)        memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

int
udp_server(const char *host, const char *serv, socklen_t *addrlenp)
{
	int				sockfd, n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0) {
		fprintf(stderr, "udp_server error for %s, %s: %s\n",
				 host, serv, gai_strerror(n));
		exit(1);
	}
	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd < 0)
			continue;		/* error - try next one */

		if (bind(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break;			/* success */

		if (close(sockfd) == -1) {		/* bind error - close and try next one */
			perror("close error");
			exit(1);
		}
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL) {	/* errno from final socket() or bind() */
		fprintf(stderr, "udp_server error for %s, %s: %s\n", 
			host, serv, strerror(errno));
		exit(1);
	}

	if (addrlenp)
		*addrlenp = res->ai_addrlen;	/* return size of protocol address */

	freeaddrinfo(ressave);

	return(sockfd);
}
/* end udp_server */

