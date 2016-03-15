#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

ssize_t	dg_send_recv(int, const void *, size_t, void *, size_t,
				   const SA *, socklen_t);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	ssize_t	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		if ((n = dg_send_recv(sockfd, sendline, strlen(sendline),
						 recvline, MAXLINE, pservaddr, servlen)) < 0) {
			perror("dg_send_recv error");
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
