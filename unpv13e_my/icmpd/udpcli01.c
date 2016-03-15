#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

int
udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen);

int
main(int argc, char **argv)
{
	int					sockfd;
	socklen_t			salen;
	struct sockaddr		*sa;

	if (argc != 3) {
		fprintf(stderr, "usage: udpcli01 <hostname> <service>\n");
		exit(1);
	}

	sockfd = udp_client(argv[1], argv[2], &sa, &salen);

	dg_cli(stdin, sockfd, sa, salen);

	exit(0);
}
