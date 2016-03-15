/* include udpservselect01 */
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef HAVE_BZERO
#define bzero(ptr,n)        memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
 *    kernels still #define it as 5, while actually supporting many more */
#define LISTENQ     1024    /* 2nd argument to listen() */

#define SERV_PORT        9877           /* TCP and UDP */

#define max(a,b)    ((a) > (b) ? (a) : (b))

void
str_echo(int sockfd);

typedef void    Sigfunc(int);   /* for signal handlers */

Sigfunc *
signal(int signo, Sigfunc *func);

int
main(int argc, char **argv)
{
	int					listenfd, connfd, udpfd, nready, maxfdp1;
	char				mesg[MAXLINE];
	pid_t				childpid;
	fd_set				rset;
	ssize_t				n;
	socklen_t			len;
	const int			on = 1;
	struct sockaddr_in	cliaddr, servaddr;
	void				sig_chld(int);

		/* 4create listening TCP socket */
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
		perror("setsockopt error");
		exit(1);
	}
	if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("bind error");
		exit(1);
	}

	if (listen(listenfd, LISTENQ) < 0) {
		perror("listen error");
		exit(1);
	}

		/* 4create UDP socket */
	if ((udpfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	if (bind(udpfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("bind error");
		exit(1);
	}
/* end udpservselect01 */

/* include udpservselect02 */
	if (signal(SIGCHLD, sig_chld) == SIG_ERR) {	/* must call waitpid() */
		perror("signal error");
		exit(1);
	}

	FD_ZERO(&rset);
	maxfdp1 = max(listenfd, udpfd) + 1;
	for ( ; ; ) {
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);
		if ( (nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0) {
			if (errno == EINTR) {
				continue;		/* back to for() */
			} else {
				perror("select error");
				exit(1);
			}
		}

		if (FD_ISSET(listenfd, &rset)) {
			len = sizeof(cliaddr);
			if ((connfd = accept(listenfd, (SA *) &cliaddr, &len)) < 0) {
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
				str_echo(connfd);	/* process the request */
				exit(0);
			} 
			if (close(connfd) == -1) {	/* parent closes connected socket */
				perror("close error");
				exit(1);
			}
		}

		if (FD_ISSET(udpfd, &rset)) {
			len = sizeof(cliaddr);
			if ((n = recvfrom(udpfd, mesg, MAXLINE, 0, (SA *) &cliaddr, &len)) < 0) {
				perror("recvfrom error");
				exit(1);
			}

			if (sendto(udpfd, mesg, n, 0, (SA *) &cliaddr, len) != n) {
				perror("sendto error");
				exit(1);
			}
		}
	}
}
/* end udpservselect02 */
