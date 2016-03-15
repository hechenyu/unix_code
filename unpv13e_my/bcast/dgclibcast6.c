#include <sys/socket.h>
#include <unistd.h>

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

#define max(a, b) ((a) < (b) ? (b) : (a))

char *
sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

static void	recvfrom_alarm(int);
static int	pipefd[2];

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n, maxfdp1;
	const int		on = 1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	fd_set			rset;
	socklen_t		len;
	struct sockaddr	*preply_addr;
 
	if ((preply_addr = malloc(servlen)) < 0) {
		perror("malloc error");
		exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	if (pipe(pipefd) < 0) {
		perror("pipe error");
		exit(1);
	}
	maxfdp1 = max(sockfd, pipefd[0]) + 1;

	FD_ZERO(&rset);

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
			FD_SET(sockfd, &rset);
			FD_SET(pipefd[0], &rset);
			if ( (n = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
				if (errno == EINTR) {
					continue;
				} else {
					perror("select error");
					exit(1);
				}
			}

			if (FD_ISSET(sockfd, &rset)) {
				len = servlen;
				if ((n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len)) < 0) {
					perror("recvfrom error");
					exit(1);
				}
				recvline[n] = 0;	/* null terminate */
				printf("from %s: %s",
						sock_ntop_host(preply_addr, len), recvline);
			}

			if (FD_ISSET(pipefd[0], &rset)) {
				if (read(pipefd[0], &n, 1) == -1) {		/* timer expired */
					perror("read error");
					exit(1);
				}
				break;
			}
		}
	}
	if (ferror(fp)) {
		perror("fgets error");
		exit(1);
	}
	free(preply_addr);
}

static void
recvfrom_alarm(int signo)
{
	if (write(pipefd[1], "", 1) != 1) {	/* write one null byte to pipe */
		perror("write error");
		exit(1);
	}
	return;
}
