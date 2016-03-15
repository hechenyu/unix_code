#include <sys/socket.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int		n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

	if (connect(sockfd, (SA *) pservaddr, servlen) < 0) {
		perror("connect error");
		exit(1);
	}

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		n = strlen(sendline);
		if (write(sockfd, sendline, n) != n) {
			perror("write error");
			exit(1);
		}

		if ((n = read(sockfd, recvline, MAXLINE)) == -1) {
			perror("read error");
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
