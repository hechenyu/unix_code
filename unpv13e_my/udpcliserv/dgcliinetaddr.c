#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr_in	*replyaddr;

	if ((replyaddr = malloc(servlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		n = strlen(sendline);
		if (sendto(sockfd, sendline, n, 0, pservaddr, servlen) != n) {
			perror("sendto error");
			exit(1);
		}

		len = servlen;
		if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, (SA *) replyaddr, &len)) < 0) {
			perror("recvfrom error");
			exit(1);
		}
		printf("received reply from %s, port %d\n",
			   inet_ntoa(replyaddr->sin_addr), htons(replyaddr->sin_port));

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
