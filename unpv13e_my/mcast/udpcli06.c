#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

int
udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen);

void
sock_set_port(struct sockaddr *sa, socklen_t salen, int port);

int
main(int argc, char **argv)
{
	int				sockfd;
	socklen_t		salen;
	struct sockaddr	*cli, *serv;

	if (argc != 2) {
		fprintf(stderr, "usage: udpcli06 <IPaddress>\n");
		exit(1);
	}

	sockfd = udp_client(argv[1], "daytime", &serv, &salen);

	if ((cli = malloc(salen)) == NULL) {
		perror("malloc error");
		exit(1);
	}
	memcpy(cli, serv, salen);		/* copy socket address struct */
	sock_set_port(cli, salen, 0);	/* and set port to 0 */
	if (bind(sockfd, cli, salen) < 0) {
		perror("bind error");
		exit(1);
	}

	dg_cli(stdin, sockfd, serv, salen);

	exit(0);
}
