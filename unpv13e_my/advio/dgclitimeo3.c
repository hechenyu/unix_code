#include <sys/socket.h>
#include <unistd.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

static void	sig_alrm(int);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int	n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];

	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	while (fgets(sendline, MAXLINE, fp) != NULL) {

		n = strlen(sendline);
		if (sendto(sockfd, sendline, n, 0, pservaddr, servlen) != n) {
			perror("sendto error");
			exit(1);
		}

		alarm(5);
		if ( (n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0) {
			if (errno == EINTR)
				fprintf(stderr, "socket timeout\n");
			else {
				perror("recvfrom error");
				exit(1);
			}
		} else {
			alarm(0);
			recvline[n] = 0;	/* null terminate */
			if (fputs(recvline, stdout) == EOF) {
				perror("fputs error");
				exit(1);
			}
		}
	}
	if (ferror(fp)) {
		perror("fgets error");
		exit(1);
	}
}

static void
sig_alrm(int signo)
{
	return;			/* just interrupt the recvfrom() */
}
