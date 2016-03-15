#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
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

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

void
sock_set_addr(struct sockaddr *sa, socklen_t salen, const void *addr);

void
sock_set_port(struct sockaddr *sa, socklen_t salen, int port);

int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	struct sockaddr_in6	servaddr6;
	struct sockaddr		*sa;
	socklen_t			salen;
	struct in_addr		**pptr;
	struct hostent		*hp;
	struct servent		*sp;

	if (argc != 3) {
		fprintf(stderr, "usage: daytimetcpcli3 <hostname> <service>\n");
		exit(1);
	}

	if ( (hp = gethostbyname(argv[1])) == NULL) {
		fprintf(stderr, "hostname error for %s: %s\n", argv[1], hstrerror(h_errno));
		exit(1);
	}

	if ( (sp = getservbyname(argv[2], "tcp")) == NULL) {
		fprintf(stderr, "getservbyname error for %s\n", argv[2]);
		exit(1);
	}

	pptr = (struct in_addr **) hp->h_addr_list;
	for ( ; *pptr != NULL; pptr++) {
		if ((sockfd = socket(hp->h_addrtype, SOCK_STREAM, 0)) < 0) {
			perror("socket error");
			exit(1);
		}

		if (hp->h_addrtype == AF_INET) {
			sa = (SA *) &servaddr;
			salen = sizeof(servaddr);
		} else if (hp->h_addrtype == AF_INET6) {
			sa = (SA *) &servaddr6;
			salen = sizeof(servaddr6);
		} else {
			fprintf(stderr, "unknown addrtype %d\n", hp->h_addrtype);
			exit(1);
		}

		bzero(sa, salen);
		sa->sa_family = hp->h_addrtype;
		sock_set_port(sa, salen, sp->s_port);
		sock_set_addr(sa, salen, *pptr);

		printf("trying %s\n", sock_ntop(sa, salen));

		if (connect(sockfd, sa, salen) == 0)
			break;		/* success */
		perror("connect error");
		close(sockfd);
	}
	if (*pptr == NULL) {
		fprintf(stderr, "unable to connect\n");
		exit(1);
	}

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF) {
			perror("fputs error");
			exit(1);
		}
	}
	if (n < 0) {
		perror("read error");
		exit(1);
	}
	exit(0);
}
