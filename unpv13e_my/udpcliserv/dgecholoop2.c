#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

static void	recvfrom_int(int);
static int	count;

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int			n;
	socklen_t	len;
	char		mesg[MAXLINE];

	if (signal(SIGINT, recvfrom_int) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	n = 220 * 1024;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	for ( ; ; ) {
		len = clilen;
		if (recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len) < 0) {
			perror("recvfrom error");
			exit(1);
		}

		count++;
	}
}

static void
recvfrom_int(int signo)
{
	printf("\nreceived %d datagrams\n", count);
	exit(0);
}
