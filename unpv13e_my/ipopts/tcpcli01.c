#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define SERV_PORT_STR        "9877"           /* TCP and UDP */

void
str_cli(FILE *fp, int sockfd);

u_char *
inet_srcrt_init(int type);

int
inet_srcrt_add(char *hostptr);

struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype, int *n);

int
main(int argc, char **argv)
{
	int					c, sockfd, len = 0;
	u_char				*ptr = NULL;
	struct addrinfo		*ai;
	int					n;

	if (argc < 2) {
		fprintf(stderr, "usage: tcpcli01 [ -[gG] <hostname> ... ] <hostname>");
		exit(1);
	}

	opterr = 0;		/* don't want getopt() writing to stderr */
	while ( (c = getopt(argc, argv, "gG")) != -1) {
		switch (c) {
		case 'g':			/* loose source route */
			if (ptr) {
				fprintf(stderr, "can't use both -g and -G\n");
				exit(1);
			}
			ptr = inet_srcrt_init(0);
			break;

		case 'G':			/* strict source route */
			if (ptr) {
				fprintf(stderr, "can't use both -g and -G\n");
				exit(1);
			}
			ptr = inet_srcrt_init(1);
			break;

		case '?':
			fprintf(stderr, "unrecognized option: %c\n", c);
			exit(1);
		}
	}

	if (ptr)
		while (optind < argc-1)
			len = inet_srcrt_add(argv[optind++]);
	else
		if (optind < argc-1) {
			fprintf(stderr, "need -g or -G to specify route\n");
			exit(1);
		}

	if (optind != argc-1) {
		fprintf(stderr, "missing <hostname>\n");
		exit(1);
	}

	if ((ai = host_serv(argv[optind], SERV_PORT_STR, AF_INET, SOCK_STREAM, &n)) == NULL) {
		fprintf(stderr, "host_serv error for %s, %s: %s",
				 (argv[optind] == NULL) ? "(no hostname)" : argv[optind],
				 (SERV_PORT_STR == NULL) ? "(no service name)" : SERV_PORT_STR,
				 gai_strerror(n));
		exit(1);
	}

	if ((sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) < 0) {
		perror("socket error");
		exit(1);
	}

	if (ptr) {
		len = inet_srcrt_add(argv[optind]);	/* dest at end */
		if (setsockopt(sockfd, IPPROTO_IP, IP_OPTIONS, ptr, len) < 0) {
			perror("setsockopt error");
			exit(1);
		}
		free(ptr);
	}

	if (connect(sockfd, ai->ai_addr, ai->ai_addrlen) < 0) {
		perror("connect error");
		exit(1);
	}

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
