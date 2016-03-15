#include <sys/socket.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE     4096    /* max text line length */

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

void
recv_all(int recvfd, socklen_t salen)
{
	int					n;
	char				line[MAXLINE+1];
	socklen_t			len;
	struct sockaddr		*safrom;

	if ((safrom = malloc(salen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	for ( ; ; ) {
		len = salen;
		if ((n = recvfrom(recvfd, line, MAXLINE, 0, safrom, &len)) < 0) {
			perror("recvfrom error");
			exit(1);
		}

		line[n] = 0;	/* null terminate */
		printf("from %s: %s", sock_ntop(safrom, len), line);
	}
}
