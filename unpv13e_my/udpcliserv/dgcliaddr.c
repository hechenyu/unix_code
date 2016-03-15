#include <sys/socket.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr	*preply_addr;

	if ((preply_addr = malloc(servlen)) == NULL) {
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
		if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len)) < 0) {
			perror("recvfrom error");
			exit(1);
		}
		if (len != servlen || memcmp(pservaddr, preply_addr, len) != 0) {
			printf("reply from %s (ignored)\n",
					sock_ntop(preply_addr, len));
			continue;
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
