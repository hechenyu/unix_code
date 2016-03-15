#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int		listenfd, connfd;

void	sig_urg(int);

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int		n;
	char	buff[100];

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], NULL);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], NULL);
	} else {
		fprintf(stderr, "usage: tcprecv01 [ <host> ] <port#>\n");
		exit(1);
	}

	if ((connfd = accept(listenfd, NULL, NULL)) < 0) {
		perror("accept error");
		exit(1);
	}

	if (signal(SIGURG, sig_urg) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	if (fcntl(connfd, F_SETOWN, getpid()) == -1) {
		perror("fcntl error");
		exit(1);
	}

	for ( ; ; ) {
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

void
sig_urg(int signo)
{
	int		n;
	char	buff[100];

	printf("SIGURG received\n");
	if ((n = recv(connfd, buff, sizeof(buff)-1, MSG_OOB)) < 0) {
		perror("recv error");
		exit(1);
	}
	buff[n] = 0;		/* null terminate */
	printf("read %d OOB byte: %s\n", n, buff);
}
