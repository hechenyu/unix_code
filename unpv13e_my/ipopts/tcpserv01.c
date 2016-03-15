#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef HAVE_BZERO
#define bzero(ptr,n)        memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
 *    kernels still #define it as 5, while actually supporting many more */
#define LISTENQ     1024    /* 2nd argument to listen() */

#define SERV_PORT        9877           /* TCP and UDP */

void
str_echo(int sockfd);

void
inet_srcrt_print(u_char *ptr, int len);

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	u_char				*opts;
	pid_t				childpid;
	socklen_t			clilen, len;
	struct sockaddr_in	cliaddr, servaddr;
	void				sig_chld(int);

	if ((opts = malloc(44)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("bind error");
		exit(1);
	}

	if (listen(listenfd, LISTENQ) < 0) {
		perror("listen error");
		exit(1);
	}

	if (signal(SIGCHLD, sig_chld) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
			if (errno == EINTR) {
				continue;		/* back to for() */
			} else {
				perror("accept error");
				exit(1);
			}
		}

		len = 44;
		if (getsockopt(connfd, IPPROTO_IP, IP_OPTIONS, opts, &len) < 0) {
			perror("getsockopt error");
			exit(1);
		}
		if (len > 0) {
			printf("received IP options, len = %d\n", len);
			inet_srcrt_print(opts, len);
		}

		if ( (childpid = fork()) < 0) {
			perror("fork error");
			exit(1);
		} else if (childpid == 0) {	/* child process */
			if (close(listenfd) == -1) {	/* close listening socket */
				perror("close error");
				exit(1);
			}
			str_echo(connfd);	/* process the request */
			exit(0);
		}
		if (close(connfd) == -1) {			/* parent closes connected socket */
			perror("close error");
			exit(1);
		}
	}
}
