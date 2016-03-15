#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sntp.h"

#define MAXLINE     4096    /* max text line length */

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

int
udp_client(const char *host, const char *serv, SA **saptr, socklen_t *lenp);

void
sock_set_wild(struct sockaddr *sa, socklen_t salen);

struct ifi_info *
get_ifi_info(int family, int doaliases);

int
mcast_join(int sockfd, const SA *grp, socklen_t grplen,
		   const char *ifname, u_int ifindex);

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

int
main(int argc, char **argv)
{
	int					sockfd;
	char				buf[MAXLINE];
	ssize_t				n;
	socklen_t			salen, len;
	struct ifi_info		*ifi;
	struct sockaddr		*mcastsa, *wild, *from;
	struct timeval		now;

	if (argc != 2) {
		fprintf(stderr, "usage: ssntp <IPaddress>\n");
		exit(1);
	}

	sockfd = udp_client(argv[1], "ntp", &mcastsa, &salen);

	if ((wild = malloc(salen)) == NULL) {
		perror("malloc error");
		exit(1);
	}
	memcpy(wild, mcastsa, salen);	/* copy family and port */
	sock_set_wild(wild, salen);
	if (bind(sockfd, wild, salen) < 0) {	/* bind wildcard */
		perror("bind error");
		exit(1);
	}

		/* 4obtain interface list and process each one */
	for (ifi = get_ifi_info(mcastsa->sa_family, 1); ifi != NULL;
		 ifi = ifi->ifi_next) {
		if (ifi->ifi_flags & IFF_MULTICAST) {
			if (mcast_join(sockfd, mcastsa, salen, ifi->ifi_name, 0) < 0) {
				perror("mcast_join error");
				exit(1);
			}
			printf("joined %s on %s\n",
				   sock_ntop(mcastsa, salen), ifi->ifi_name);
		}
	}

	if ((from = malloc(salen)) == NULL) {
		perror("malloc error");
		exit(1);
	}
	for ( ; ; ) {
		len = salen;
		if ((n = recvfrom(sockfd, buf, sizeof(buf), 0, from, &len)) < 0) {
			perror("recvfrom error");
			exit(1);
		}
		if (gettimeofday(&now, NULL) == -1) {
			perror("gettimeofday error");
			exit(1);
		}
		sntp_proc(buf, n, &now);
	}
}
