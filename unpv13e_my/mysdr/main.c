#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define	SAP_NAME	"sap.mcast.net"	/* default group name and port */
#define	SAP_PORT	"9875"

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

int
mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex);

int
udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp);

void	loop(int, socklen_t);

int
main(int argc, char **argv)
{
	int					sockfd;
	const int			on = 1;
	socklen_t			salen;
	struct sockaddr		*sa;

	if (argc == 1) {
		sockfd = udp_client(SAP_NAME, SAP_PORT, &sa, &salen);
	} else if (argc == 4) {
		sockfd = udp_client(argv[1], argv[2], &sa, &salen);
	} else {
		fprintf(stderr, "usage: mysdr <mcast-addr> <port#> <interface-name>\n");
		exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		perror("setsockopt error");
		exit(1);
	}
	if (bind(sockfd, sa, salen) < 0) {
		perror("bind error");
		exit(1);
	}

	if (mcast_join(sockfd, sa, salen, (argc == 4) ? argv[3] : NULL, 0) < 0) {
		perror("mcast_join error");
		exit(1);
	}

	loop(sockfd, salen);	/* receive and print */

	exit(0);
}
