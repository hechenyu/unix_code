#include <sys/socket.h>
#include <unistd.h>
#include <setjmp.h>

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

char *
sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

static void			recvfrom_alarm(int);
static sigjmp_buf	jmpbuf;

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	if ((preply_addr = malloc(servlen)) < 0) {
		perror("malloc error");
		exit(1);
	}

	if (signal(SIGALRM, recvfrom_alarm) < 0) {
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
		for ( ; ; ) {
			if (sigsetjmp(jmpbuf, 1) != 0)
				break;
			len = servlen;
			if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len)) < 0) {
				perror("recvfrom error");
				exit(1);
			}
			recvline[n] = 0;	/* null terminate */
			printf("from %s: %s",
					sock_ntop_host(preply_addr, servlen), recvline);
		}
	}
	if (ferror(fp)) {
		perror("fgets error");
		exit(1);
	}
}

static void
recvfrom_alarm(int signo)
{
	siglongjmp(jmpbuf, 1);
}
