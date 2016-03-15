/* include serv08 */
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "pthread08.h"

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

static int			nthreads;
pthread_mutex_t		clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t		clifd_cond = PTHREAD_COND_INITIALIZER;

int
main(int argc, char **argv)
{
	int			i, listenfd, connfd;
	void		sig_int(int), thread_make(int);
	socklen_t	addrlen, clilen;
	struct sockaddr	*cliaddr;
	int n;

	if (argc == 3) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 4) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: serv08 [ <host> ] <port#> <#threads>\n");
		exit(1);
	}
	if ((cliaddr = malloc(addrlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	nthreads = atoi(argv[argc-1]);
	if ((tptr = calloc(nthreads, sizeof(Thread))) == NULL) {
		perror("calloc error");
		exit(1);
	}
	iget = iput = 0;

		/* 4create all the threads */
	for (i = 0; i < nthreads; i++)
		thread_make(i);		/* only main thread returns */

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

		if ((n = pthread_mutex_lock(&clifd_mutex)) != 0) {
			errno = n;
			perror("pthread_mutex_lock error");
			exit(1);
		}
		clifd[iput] = connfd;
		if (++iput == MAXNCLI)
			iput = 0;
		if (iput == iget) {
			fprintf(stderr, "iput = iget = %d\n", iput);
			exit(1);
		}
		if ((n = pthread_cond_signal(&clifd_cond)) != 0) {
			errno = n;
			perror("pthread_cond_signal error");
			exit(1);
		}
		if ((n = pthread_mutex_unlock(&clifd_mutex)) != 0) {
			errno = n;
			perror("pthread_mutex_unlock error");
			exit(1);
		}
	}
}
/* end serv08 */

void
sig_int(int signo)
{
	int		i;
	void	pr_cpu_time(void);

	pr_cpu_time();

	for (i = 0; i < nthreads; i++)
		printf("thread %d, %ld connections\n", i, tptr[i].thread_count);

	exit(0);
}
