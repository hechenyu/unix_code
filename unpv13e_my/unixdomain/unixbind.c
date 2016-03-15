#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

int
main(int argc, char **argv)
{
	int					sockfd;
	socklen_t			len;
	struct sockaddr_un	addr1, addr2;

	if (argc != 2) {
		fprintf(stderr, "usage: unixbind <pathname>\n");
		exit(1);
	}

	if ((sockfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	unlink(argv[1]);		/* OK if this fails */

	bzero(&addr1, sizeof(addr1));
	addr1.sun_family = AF_LOCAL;
	strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path)-1);
	if (bind(sockfd, (SA *) &addr1, SUN_LEN(&addr1)) < 0) {
		perror("bind error");
		exit(1);
	}

	len = sizeof(addr2);
	if (getsockname(sockfd, (SA *) &addr2, &len) < 0) {
		perror("getsockname error");
		exit(1);
	}
	printf("bound name = %s, returned len = %d\n", addr2.sun_path, len);
	
	exit(0);
}
