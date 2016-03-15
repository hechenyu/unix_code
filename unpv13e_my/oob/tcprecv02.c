#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int		listenfd, connfd, n;
	char	buff[100];
	fd_set	rset, xset;

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], NULL);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], NULL);
	} else {
		fprintf(stderr, "usage: tcprecv02 [ <host> ] <port#>\n");
		exit(1);
	}

	if ((connfd = accept(listenfd, NULL, NULL)) < 0) {
		perror("accept error");
		exit(1);
	}

	FD_ZERO(&rset);
	FD_ZERO(&xset);
	for ( ; ; ) {
		FD_SET(connfd, &rset);
		FD_SET(connfd, &xset);

		if (select(connfd + 1, &rset, NULL, &xset, NULL) < 0) {
			perror("select error");
			exit(1);
		}

		if (FD_ISSET(connfd, &xset)) {
			if ((n = recv(connfd, buff, sizeof(buff)-1, MSG_OOB)) < 0) {
				perror("recv error");
				exit(1);
			}
			buff[n] = 0;		/* null terminate */
			printf("read %d OOB byte: %s\n", n, buff);
		}

		if (FD_ISSET(connfd, &rset)) {
			if ( (n = read(connfd, buff, sizeof(buff)-1)) < 0) {
				perror("read error");
				exit(1);
			} else if (n == 0) {
				printf("received EOF\n");
				exit(0);
			}
			buff[n] = 0;	/* null terminate */
			printf("read %d bytes: %s\n", n, buff);
		}
	}
}
