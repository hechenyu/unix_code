#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

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

#define UNIXSTR_PATH    "/tmp/unix.str" /* Unix domain stream */

void
str_echo(int sockfd);

typedef void    Sigfunc(int);   /* for signal handlers */

Sigfunc *
signal(int signo, Sigfunc *func);

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_un	cliaddr, servaddr;
	void				sig_chld(int);

	if ((listenfd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	unlink(UNIXSTR_PATH);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family      = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIXSTR_PATH);

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
		if ((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
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
			str_echo(connfd);	/* process request */
			exit(0);
		} 
		if (close(connfd) == -1) {	/* parent closes connected socket */
			perror("close error");
			exit(1);
		}
	}
}
