#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static void	*doit(void *);		/* each thread executes this function */

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

void
str_echo(int sockfd);

int
main(int argc, char **argv)
{
	int				listenfd, connfd;
	pthread_t		tid;
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;
	int n;

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: tcpserv01 [ <host> ] <service or port>\n");
		exit(1);
	}

	if ((cliaddr = malloc(addrlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	for ( ; ; ) {
		len = addrlen;
		if ((connfd = accept(listenfd, cliaddr, &len)) < 0) {
			perror("accept error");
			exit(1);
		}
		if ((n = pthread_create(&tid, NULL, &doit, (void *) connfd)) != 0) {
			errno = n;
			perror("pthread_create error");
			exit(1);
		}
	}
}

static void *
doit(void *arg)
{
	int n;
	if ((n = pthread_detach(pthread_self())) != 0) {
		errno = n;
		perror("pthread_detach error");
		exit(1);
	}
	str_echo((int) arg);	/* same function as before */
	if (close((int) arg) == -1) {		/* done with connected socket */
		perror("close error");
		exit(1);
	}
	return(NULL);
}
