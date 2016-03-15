/* include udp_connect */
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
udp_connect(const char *host, const char *serv)
{
	int				sockfd, n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0) {
		fprintf(stderr, "udp_connect error for %s, %s: %s\n",
				 host, serv, gai_strerror(n));
		exit(1);
	}
	ressave = res;

	do {
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd < 0)
			continue;	/* ignore this one */

		if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break;		/* success */

		if (close(sockfd) == -1) {	/* ignore this one */
			perror("close error");
			exit(1);
		}
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL) {	/* errno set from final connect() */
		fprintf(stderr, "udp_connect error for %s, %s: %s\n",
			host, serv, strerror(errno));
		exit(1);
	}

	freeaddrinfo(ressave);

	return(sockfd);
}
/* end udp_connect */

