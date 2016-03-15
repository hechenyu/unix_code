#include <stdio.h>
#include <stdlib.h>

int
tcp_connect(const char *host, const char *serv);

void
str_cli(FILE *fp_arg, int sockfd_arg);

int
main(int argc, char **argv)
{
	int		sockfd;

	if (argc != 3) {
		fprintf(stderr, "usage: tcpcli <hostname> <service>\n");
		exit(1);
	}

	sockfd = tcp_connect(argv[1], argv[2]);

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
