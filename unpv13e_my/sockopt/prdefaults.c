#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

static void doit(int, const char *);

int main()
{
	int		tcpsock, udpsock;
	struct sockaddr_in	servaddr;

	if ( (tcpsock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("TCP socket error");
		exit(1);
	}

#ifdef nodef
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_port        = htons(9);
	if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) != 1) {
		perror("inet_pton error");
		exit(1);
	}

	if (connect(tcpsock, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("connect error");
		exit(1);
	}
#endif

	doit(tcpsock, "tcp");

	if ( (udpsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("UDP socket error");
		exit(1);
	}

	doit(udpsock, "udp");
	exit(0);
}

static
void doit(int fd, const char *name)
{
	int			val;
	socklen_t	optlen;

	optlen = sizeof(val);
	if (getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &val, &optlen) < 0) {
		perror("SO_SNDBUF getsockopt error");
		exit(1);
	}
	printf("%s send buffer size = %d\n", name, val);

	optlen = sizeof(val);
	if (getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &val, &optlen) < 0) {
		perror("SO_RCVBUF getsockopt error");
		exit(1);
	}
	printf("%s recv buffer size = %d\n", name, val);

	optlen = sizeof(val);
	if (getsockopt(fd, SOL_SOCKET, SO_SNDLOWAT, &val, &optlen) < 0) {
		perror("SO_SNDLOWAT getsockopt error");
		exit(1);
	}
	printf("%s send low-water mark = %d\n", name, val);

	optlen = sizeof(val);
	if (getsockopt(fd, SOL_SOCKET, SO_RCVLOWAT, &val, &optlen) < 0) {
		perror("SO_RCVLOWAT getsockopt error");
		exit(1);
	}
	printf("%s receive low-water mark size = %d\n", name, val);
}
