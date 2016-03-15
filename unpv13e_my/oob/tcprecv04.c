#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int		listenfd, connfd, n, on=1;
	char	buff[100];

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], NULL);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], NULL);
	} else {
		fprintf(stderr, "usage: tcprecv04 [ <host> ] <port#>\n");
		exit(1);
	}

	if (setsockopt(listenfd, SOL_SOCKET, SO_OOBINLINE, &on, sizeof(on)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	if ((connfd = accept(listenfd, NULL, NULL)) < 0) {
		perror("accept error");
		exit(1);
	}
	sleep(5);

	for ( ; ; ) {
		if ((n = sockatmark(connfd)) < 0) {
			perror("sockatmark error");
			exit(1);
		} else if (n) {
			printf("at OOB mark\n");
		}

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
