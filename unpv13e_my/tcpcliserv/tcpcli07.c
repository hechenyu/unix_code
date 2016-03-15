#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>

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

#define SERV_PORT        9877           /* TCP and UDP */

void
str_cli(FILE *fp, int sockfd);

int
main(int argc, char **argv)
{
	int					sockfd;
	void				sig_alrm(int);
	struct itimerval	val;
	struct sockaddr_in	servaddr;
	int ret;

	if (argc != 2) {
		fprintf(stderr, "usage: tcpcli <IPaddress>\n");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	if ((ret = inet_pton(AF_INET, argv[1], &servaddr.sin_addr)) < 0) {
		fprintf(stderr, "inet_pton error for %s: %s\n",
			argv[1], strerror(errno));	/* errno set */
		exit(1);
	} else if (ret == 0) {
		fprintf(stderr, "inet_pton error for %s\n", argv[1]);	/* errno not set */
		exit(1);
	}

		/* Set interval timer to go off before 3WHS completes */
	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	val.it_interval.tv_sec  = 0;
	val.it_interval.tv_usec = 0;
	val.it_value.tv_sec  = 0;
	val.it_value.tv_usec = 50000;	/* 50 ms */
	if (setitimer(ITIMER_REAL, &val, NULL) == -1) {
		perror("setitimer error");
		exit(1);
	}

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
		perror("connect error");
		exit(1);
	}

	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}

void
sig_alrm(int signo)
{
	exit(0);
}
