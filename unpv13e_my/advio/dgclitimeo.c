#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

int
readable_timeo(int fd, int sec);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		n = strlen(sendline);
		if (sendto(sockfd, sendline, n, 0, pservaddr, servlen) != n) {
			perror("sendto error");
			exit(1);
		}

		if ((n = readable_timeo(sockfd, 5)) < 0) {
			perror("readable_timeo error");
			exit(1);
	 	} else if (n == 0) {
			fprintf(stderr, "socket timeout\n");
			continue;
		}
		if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0) {
			perror("recvfrom error");
			exit(1);
		}
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF) {
			perror("fputs error");
			exit(1);
		}
	}
	if (ferror(fp)) {
		perror("fgets error");
		exit(1);
	}
}
