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

int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	struct in_addr		**pptr;
	struct in_addr		*inetaddrp[2];
	struct in_addr		inetaddr;
	struct hostent		*hp;
	struct servent		*sp;

	if (argc != 3) {
		fprintf(stderr, "usage: daytimetcpcli1 <hostname> <service>\n");
		exit(1);
	}

	if ( (hp = gethostbyname(argv[1])) == NULL) {
		if (inet_aton(argv[1], &inetaddr) == 0) {
			fprintf(stderr, "hostname error for %s: %s\n", argv[1], hstrerror(h_errno));
		} else {
			inetaddrp[0] = &inetaddr;
			inetaddrp[1] = NULL;
			pptr = inetaddrp;
		}
	} else {
		pptr = (struct in_addr **) hp->h_addr_list;
	}

	if ( (sp = getservbyname(argv[2], "tcp")) == NULL) {
		fprintf(stderr, "getservbyname error for %s\n", argv[2]);
		exit(1);
	}

	for ( ; *pptr != NULL; pptr++) {
		if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			perror("socket error");
			exit(1);
		}

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = sp->s_port;
		memcpy(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));
		printf("trying %s\n",
			   sock_ntop((SA *) &servaddr, sizeof(servaddr)));

		if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == 0)
			break;		/* success */
		perror("connect error");
		close(sockfd);
	}
	if (*pptr == NULL) {
		fprintf(stderr, "unable to connect");
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
