/* include readable_conn1 */
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "icmpd.h"

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

ssize_t
read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);

int
sock_get_port(const struct sockaddr *sa, socklen_t salen);

int
sock_bind_wild(int sockfd, int family);

int
readable_conn(int i)
{
	int				unixfd, recvfd;
	char			c;
	ssize_t			n;
	socklen_t		len;
	struct sockaddr_storage	ss;

	unixfd = client[i].connfd;
	recvfd = -1;
	if ( (n = read_fd(unixfd, &c, 1, &recvfd)) < 0) {
		perror("read_fd error");
		exit(1);
	} else if (n == 0) {
		fprintf(stderr, "client %d terminated, recvfd = %d\n", i, recvfd);
		goto clientdone;	/* client probably terminated */
	}

		/* 4data from client; should be descriptor */
	if (recvfd < 0) {
		fprintf(stderr, "read_fd did not return descriptor\n");
		goto clienterr;
	}
/* end readable_conn1 */

/* include readable_conn2 */
	len = sizeof(ss);
	if (getsockname(recvfd, (SA *) &ss, &len) < 0) {
		perror("getsockname error");
		goto clienterr;
	}

	client[i].family = ss.ss_family;
	if ( (client[i].lport = sock_get_port((SA *)&ss, len)) == 0) {
		client[i].lport = sock_bind_wild(recvfd, client[i].family);
		if (client[i].lport <= 0) {
			perror("error binding ephemeral port");
			goto clienterr;
		}
	}
	if (write(unixfd, "1", 1) != 1) {	/* tell client all OK */
		perror("write error");
		exit(1);
	}
	if (close(recvfd) == -1) {			/* all done with client's UDP socket */
		perror("close error");
		exit(1);
	}
	return(--nready);

clienterr:
	if (write(unixfd, "0", 1) != 1) {	/* tell client error occurred */
		perror("write error");
		exit(1);
	}
clientdone:
	if (close(unixfd) == -1) {
		perror("close error");
		exit(1);
	}
	if (recvfd >= 0) {
		if (close(recvfd) == -1) {
			perror("close error");
			exit(1);
		}
	}
	FD_CLR(unixfd, &allset);
	client[i].connfd = -1;
	return(--nready);
}
/* end readable_conn2 */
