/* include serv01 */
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	void				sig_chld(int), sig_int(int), web_child(int);
	socklen_t			clilen, addrlen;
	struct sockaddr		*cliaddr;

	if (argc == 2) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 3) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: serv01 [ <host> ] <port#>\n");
		exit(1);
	}
	if ((cliaddr = malloc(addrlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	if (signal(SIGCHLD, sig_chld) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	if (signal(SIGINT, sig_int) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	for ( ; ; ) {
		clilen = addrlen;
		if ( (connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
			if (errno == EINTR) {
				continue;		/* back to for() */
			} else {
				perror("accept error");
				exit(1);
			}
		}

		if ( (childpid = fork()) < 0) {
			perror("fork error");
			exit(1);
		} else if (childpid == 0) {	/* child process */
			if (close(listenfd) == -1) {	/* close listening socket */
				perror("close error");
				exit(1);
			}
			web_child(connfd);	/* process request */
			exit(0);
		}
		if (close(connfd) == -1) {			/* parent closes connected socket */
			perror("close error");
			exit(1);
		}
	}
}
/* end serv01 */

/* include sigint */
void
sig_int(int signo)
{
	void	pr_cpu_time(void);

	pr_cpu_time();
	exit(0);
}
/* end sigint */
