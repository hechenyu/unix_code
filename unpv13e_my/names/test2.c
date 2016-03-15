#include <sys/socket.h>
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

int
udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp);

char *
sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

int
main(int argc, char **argv)
{
	int				sockfd, n;
	char			recvline[MAXLINE + 1];
	socklen_t		salen;
	struct sockaddr	*sa, *sabind;

	if (argc != 3) {
		fprintf(stderr, "usage: test2 <hostname/IPaddress> <service/port#>\n");
		exit(1);
	}

	sockfd = udp_client(argv[1], argv[2], &sa, &salen);

	/* Same as daytimeudpcli1, but we explicitly bind the wildcard */
	if ((sabind = malloc(salen)) == NULL) {
		perror("malloc error");
		exit(1);
	}
	bzero(sabind, salen);
	sabind->sa_family = sa->sa_family;
	if (bind(sockfd, sabind, salen) < 0) {
		perror("bind error");
		exit(1);
	}
	printf("bound %s\n", sock_ntop(sabind, salen));

	printf("sending to %s\n", sock_ntop_host(sa, salen));

	if (sendto(sockfd, "", 1, 0, sa, salen) != 1) {	/* send 1-byte datagram */
		perror("sendto error");
		exit(1);
	}

	if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0) {
		perror("recvfrom error");
		exit(1);
	}
	recvline[n] = 0;	/* null terminate */
	if (fputs(recvline, stdout) == EOF) {
		perror("fputs error");
		exit(1);
	}

	exit(0);
}
