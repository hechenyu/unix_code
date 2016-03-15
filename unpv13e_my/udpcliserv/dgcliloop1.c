#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define	NDG		2000	/* datagrams to send */
#define	DGLEN	1400	/* length of each datagram */

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int		i;
	char	sendline[DGLEN];

	for (i = 0; i < NDG; i++) {
		if (sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen) != DGLEN) {
			perror("sendto error");
			exit(1);
		}
	}
}
