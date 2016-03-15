/* include dgcli011 */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "unpicmpd.h"

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

#define	max(a, b)	((a) < (b) ? (b) : (a))

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

int
sock_bind_wild(int sockfd, int family);

ssize_t
write_fd(int fd, void *ptr, size_t nbytes, int sendfd);

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				icmpfd, maxfdp1;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	fd_set			rset;
	ssize_t			n;
	struct timeval	tv;
	struct icmpd_err icmpd_err;
	struct sockaddr_un sun;

	if (sock_bind_wild(sockfd, pservaddr->sa_family) < 0) {
		perror("sock_bind_wild error");
		exit(1);
	}

	if ((icmpfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}
	sun.sun_family = AF_LOCAL;
	strcpy(sun.sun_path, ICMPD_PATH);
	if (connect(icmpfd, (SA *)&sun, sizeof(sun)) < 0) {
		perror("connect error");
		exit(1);
	}
	if (write_fd(icmpfd, "1", 1, sockfd) < 0) {
		perror("write_fd error");
		exit(1);
	}
	if ((n = read(icmpfd, recvline, 1)) == -1) {
		perror("read error");
		exit(1);
	}
	if (n != 1 || recvline[0] != '1') {
		fprintf(stderr, "error creating icmp socket, n = %d, char = %c\n",
				 n, recvline[0]);
		exit(1);
	}

	FD_ZERO(&rset);
	maxfdp1 = max(sockfd, icmpfd) + 1;
/* end dgcli011 */

/* include dgcli012 */
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		n = strlen(sendline);
		if (sendto(sockfd, sendline, n, 0, pservaddr, servlen) != n) {
			perror("sendto error");
			exit(1);
		}

		tv.tv_sec = 5;
		tv.tv_usec = 0;
		FD_SET(sockfd, &rset);
		FD_SET(icmpfd, &rset);
		if ( (n = select(maxfdp1, &rset, NULL, NULL, &tv)) < 0) {
			perror("select error");
			exit(1); 
		} else if (n == 0) {
			fprintf(stderr, "socket timeout\n");
			continue;
		}

		if (FD_ISSET(sockfd, &rset)) {
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

		if (FD_ISSET(icmpfd, &rset)) {
			if ( (n = read(icmpfd, &icmpd_err, sizeof(icmpd_err))) < 0) {
				perror("read error");
				exit(1);
			} else if (n == 0) {
				fprintf(stderr, "ICMP daemon terminated\n");
				exit(1);
			} else if (n != sizeof(icmpd_err)) {
				fprintf(stderr, "n = %d, expected %d\n", n, sizeof(icmpd_err));
				exit(1);
			}
			printf("ICMP error: dest = %s, %s, type = %d, code = %d\n",
				   sock_ntop(&icmpd_err.icmpd_dest, icmpd_err.icmpd_len),
				   strerror(icmpd_err.icmpd_errno),
				   icmpd_err.icmpd_type, icmpd_err.icmpd_code);
		}
	}
}
/* end dgcli012 */
