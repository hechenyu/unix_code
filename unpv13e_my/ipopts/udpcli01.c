#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define SERV_PORT_STR        "9877"           /* TCP and UDP */

struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype, int *n);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen);

int
main(int argc, char **argv)
{
	int					c, sockfd, len = 0;
	u_char				*ptr = NULL;
	void				*rth;
	struct addrinfo		*ai;
	int					n;

	if (argc < 2) {
		fprintf(stderr, "usage: udpcli01 [ <hostname> ... ] <hostname>\n");
		exit(1);
	}

	if (argc > 2) {
		int i;

		if ((len = inet6_rth_space(IPV6_RTHDR_TYPE_0, argc-2)) < 0) {
			fprintf(stderr, "inet6_rth_space error\n");
			exit(1);
		}
		if ((ptr = malloc(len)) == NULL) {
			perror("malloc error");
			exit(1);
		}
		if (inet6_rth_init(ptr, len, IPV6_RTHDR_TYPE_0, argc-2) == NULL) {
			fprintf(stderr, "inet6_rth_init error\n");
			exit(1);
		}
		for (i = 1; i < argc-1; i++) {
			if ((ai = host_serv(argv[i], NULL, AF_INET6, 0, &n)) == NULL) {
				fprintf(stderr, "host_serv error for %s, %s: %s",
						 (argv[i] == NULL) ? "(no hostname)" : argv[i],
						 "(no service name)",
						 gai_strerror(n));
				exit(1);
			}
			if (inet6_rth_add(ptr, &((struct sockaddr_in6 *)ai->ai_addr)->sin6_addr) < 0) {
				perror("inet6_rth_add error");
				exit(1);
			}
		}
	}

	if ((ai = host_serv(argv[argc-1], SERV_PORT_STR, AF_INET6, SOCK_DGRAM, &n)) == NULL) {
		fprintf(stderr, "host_serv error for %s, %s: %s",
				 (argv[argc-1] == NULL) ? "(no hostname)" : argv[argc-1],
				 (SERV_PORT_STR == NULL) ? "(no service name)" : SERV_PORT_STR,
				 gai_strerror(n));
		exit(1);
	}

	if ((sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
		perror("socket error");
		exit(1);
	}

	if (ptr) {
		if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_RTHDR, ptr, len) < 0) {
			perror("setsockopt error");
			exit(1);
		}
		free(ptr);
	}

	dg_cli(stdin, sockfd, ai->ai_addr, ai->ai_addrlen);	/* do it all */

	exit(0);
}
