/* include serv06 */
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int				listenfd, connfd;
	void			sig_int(int);
	void			*doit(void *);
	pthread_t		tid;
	socklen_t		clilen, addrlen;
	struct sockaddr	*cliaddr;
	int				n;

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: serv06 [ <host> ] <port#>\n");
		exit(1);
	}
	if ((cliaddr = malloc(addrlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	for ( ; ; ) {
		clilen = addrlen;
		if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
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

void *
doit(void *arg)
{
	void	web_child(int);
	int n;

	if ((n = pthread_detach(pthread_self())) != 0) {
		errno = n;
		perror("pthread_detach error");
		exit(1);
	}
	web_child((int) arg);
	if (close((int) arg) == -1) {
		perror("close error");
		exit(1);
	}
	return(NULL);
}
/* end serv06 */

void
sig_int(int signo)
{
	void	pr_cpu_time(void);

	pr_cpu_time();
	exit(0);
}
