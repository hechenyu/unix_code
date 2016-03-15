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

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef HAVE_BZERO
#define bzero(ptr,n)        memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define SERV_PORT        9877           /* TCP and UDP */

#define MAXLINE     4096    /* max text line length */

void	mydg_echo(int, SA *, socklen_t, SA *);

struct ifi_info *
get_ifi_info(int family, int doaliases);

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

int
main(int argc, char **argv)
{
	int					sockfd;
	const int			on = 1;
	pid_t				pid;
	struct ifi_info		*ifi, *ifihead;
	struct sockaddr_in	*sa, cliaddr, wildaddr;

	for (ifihead = ifi = get_ifi_info(AF_INET, 1);
		 ifi != NULL; ifi = ifi->ifi_next) {

		assert(ifi != NULL);

			/*4bind unicast address */
		if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("socket error");
			exit(1);
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
			perror("setsockopt error");
			exit(1);
		}

		sa = (struct sockaddr_in *) ifi->ifi_addr;
		sa->sin_family = AF_INET;
		sa->sin_port = htons(SERV_PORT);
		if (bind(sockfd, (SA *) sa, sizeof(*sa)) < 0) {
			perror("bind error");
			exit(1);
		}
		printf("bound %s\n", sock_ntop((SA *) sa, sizeof(*sa)));

		if ( (pid = fork()) < 0) {
			perror("fork error");
			exit(1);
		} else if (pid == 0) {		/* child */
			mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
			exit(0);		/* never executed */
		}
/* end udpserv1 */
/* include udpserv2 */
		if (ifi->ifi_flags & IFF_BROADCAST) {
				/* 4try to bind broadcast address */
			if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
				perror("socket error");
				exit(1);
			}
			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
				perror("setsockopt error");
				exit(1);
			}

			sa = (struct sockaddr_in *) ifi->ifi_brdaddr;
			sa->sin_family = AF_INET;
			sa->sin_port = htons(SERV_PORT);
			if (bind(sockfd, (SA *) sa, sizeof(*sa)) < 0) {
				if (errno == EADDRINUSE) {
					printf("EADDRINUSE: %s\n",
						   sock_ntop((SA *) sa, sizeof(*sa)));
					if (close(sockfd) == -1) {
						perror("close error");
						exit(1);
					}
					continue;
				} else {
					fprintf(stderr, "bind error for %s: %s\n",
							sock_ntop((SA *) sa, sizeof(*sa)));
					exit(1);
				}
			}
			printf("bound %s\n", sock_ntop((SA *) sa, sizeof(*sa)));

			if ( (pid = fork()) < 0) {
				perror("fork error");
				exit(1);
			} else if (pid == 0) {		/* child */
				mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr),
						  (SA *) sa);
				exit(0);		/* never executed */
			}
		}
	}
/* end udpserv2 */
/* include udpserv3 */
		/* 4bind wildcard address */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	bzero(&wildaddr, sizeof(wildaddr));
	wildaddr.sin_family = AF_INET;
	wildaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	wildaddr.sin_port = htons(SERV_PORT);
	if (bind(sockfd, (SA *) &wildaddr, sizeof(wildaddr)) < 0) {
		perror("bind error");
		exit(1);
	}
	printf("bound %s\n", sock_ntop((SA *) &wildaddr, sizeof(wildaddr)));

	if ( (pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid == 0) {		/* child */
		mydg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr), (SA *) sa);
		exit(0);		/* never executed */
	}
	exit(0);
}
/* end udpserv3 */

/* include mydg_echo */
void
mydg_echo(int sockfd, SA *pcliaddr, socklen_t clilen, SA *myaddr)
{
	int			n;
	char		mesg[MAXLINE];
	socklen_t	len;

	for ( ; ; ) {
		len = clilen;
		if ((n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len)) < 0) {
			perror("recvfrom error");
			exit(1);
		}
		printf("child %d, datagram from %s", getpid(),
			   sock_ntop(pcliaddr, len));
		printf(", to %s\n", sock_ntop(myaddr, clilen));

		if (sendto(sockfd, mesg, n, 0, pcliaddr, len) != n) {
			perror("send error");
			exit(1);
		}
	}
}
/* end mydg_echo */
