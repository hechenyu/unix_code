#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

int
mcast_set_loop(int sockfd, int onoff);

int
mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex);

int
udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp);

void	recv_all(int, socklen_t);
void	send_all(int, SA *, socklen_t);

int
main(int argc, char **argv)
{
	int					sendfd, recvfd;
	const int			on = 1;
	socklen_t			salen;
	struct sockaddr		*sasend, *sarecv;
	pid_t pid;

	if (argc != 3) {
		fprintf(stderr, "usage: sendrecv <IP-multicast-address> <port#>\n");
		exit(1);
	}

	sendfd = udp_client(argv[1], argv[2], &sasend, &salen);

	if ((recvfd = socket(sasend->sa_family, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	if (setsockopt(recvfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	if ((sarecv = malloc(salen)) < 0) {
		perror("malloc error");
		exit(1);
	}
	memcpy(sarecv, sasend, salen);
	if (bind(recvfd, sarecv, salen) < 0) {
		perror("bind error");
		exit(1);
	}

	if (mcast_join(recvfd, sasend, salen, NULL, 0) < 0) {
		perror("mcast_join error");
		exit(1);
	}
	if (mcast_set_loop(sendfd, 0) < 0) {
		perror("mcast_set_loop error");
		exit(1);
	}

	if ((pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) {
		recv_all(recvfd, salen);		/* child -> receives */
	}

	send_all(sendfd, sasend, salen);	/* parent -> sends */

	return 0;
}
