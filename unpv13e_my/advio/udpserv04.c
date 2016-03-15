/* include udpserv1 */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "ifi_info.h"

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

void	mydg_echo(int, SA *, socklen_t);

int
main(int argc, char **argv)
{
	int					sockfd, family, port;
	const int			on = 1;
	pid_t				pid;
	socklen_t			salen;
	struct sockaddr		*sa, *wild;
	struct ifi_info		*ifi, *ifihead;

	if (argc == 2) {
		sockfd = udp_client(NULL, argv[1], (void **) &sa, &salen);
	} else if (argc == 3) {
		sockfd = udp_client(argv[1], argv[2], (void **) &sa, &salen);
	} else {
		fprintf(stderr, "usage: udpserv04 [ <host> ] <service or port>\n");
		exit(1);
	}
	family = sa->sa_family;
	port = sock_get_port(sa, salen);
	if (close(sockfd) == -1) {		/* we just want family, port, salen */
		perror("close error");
		exit(1);
	}

	for (ifihead = ifi = get_ifi_info(family, 1);
		 ifi != NULL; ifi = ifi->ifi_next) {

		assert(ifi != NULL);

			/*4bind unicast address */
		if ((sockfd = socket(family, SOCK_DGRAM, 0)) < 0) {
			perror("socket error");
			exit(1);
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
			perror("setsockopt error");
			exit(1);
		}

		sock_set_port(ifi->ifi_addr, salen, port);
		if (bind(sockfd, ifi->ifi_addr, salen) < 0) {
			perror("bind error");
			exit(1);
		}
		printf("bound %s\n", sock_ntop(ifi->ifi_addr, salen));

		if ( (pid = fork()) < 0) {
			perror("fork error");
			exit(1);
		} else if (pid == 0) {		/* child */
			mydg_echo(sockfd, ifi->ifi_addr, salen);
			exit(0);		/* never executed */
		}

		if (ifi->ifi_flags & IFF_BROADCAST) {
				/* 4try to bind broadcast address */
			sockfd = socket(family, SOCK_DGRAM, 0);
			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
				perror("setsockopt error");
				exit(1);
			}

			sock_set_port(ifi->ifi_brdaddr, salen, port);
			if (bind(sockfd, ifi->ifi_brdaddr, salen) < 0) {
				if (errno == EADDRINUSE) {
					printf("EADDRINUSE: %s\n",
						   sock_ntop(ifi->ifi_brdaddr, salen));
					if (close(sockfd) == -1) {
						perror("close error");
						exit(1);
					}
					continue;
				} else {
					fprintf(stderr, "bind error for %s: %s\n",
							sock_ntop(ifi->ifi_brdaddr, salen));
					exit(1);
				}
			}
			printf("bound %s\n", sock_ntop(ifi->ifi_brdaddr, salen));

			if ( (pid = fork()) < 0) {
				perror("fork error");
				exit(1);
			} else if (pid == 0) {		/* child */
				mydg_echo(sockfd, ifi->ifi_brdaddr, salen);
				exit(0);		/* never executed */
			}
		}
	}

		/* 4bind wildcard address */
	if ((sockfd = socket(family, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	if ((wild = malloc(salen)) == NULL) {
		perror("malloc error");
		exit(1);
	}
	memcpy(wild, sa, salen);	/* copy family and port */
	sock_set_wild(wild, salen);

	if (bind(sockfd, wild, salen) < 0) {
		perror("bind error");
		exit(1);
	}
	printf("bound %s\n", sock_ntop(wild, salen));

	if ( (pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) {		/* child */
		mydg_echo(sockfd, wild, salen);
		exit(0);		/* never executed */
	}
	exit(0);
}

void
mydg_echo(int sockfd, SA *myaddr, socklen_t salen)
{
	int			n;
	char		mesg[MAXLINE];
	socklen_t	len;
	struct sockaddr *cli;

	if ((cli = malloc(salen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	for ( ; ; ) {
		len = salen;
		if ((n = recvfrom(sockfd, mesg, MAXLINE, 0, cli, &len)) < 0) {
			perror("recvfrom error");
			exit(1);
		}
		printf("child %d, datagram from %s",
			   getpid(), sock_ntop(cli, len));
		printf(", to %s\n", sock_ntop(myaddr, salen));

		if (sendto(sockfd, mesg, n, 0, cli, len) != n) {
			perror("send error");
			exit(1);
		}
	}
}
