#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "pthread09.h"

void
thread_make(int i)
{
	void	*thread_main(void *);
	int		n;

	if ((n = pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *) i)) != 0) {
		errno = n;
		perror("pthread_create error");
		exit(1);
	}
	return;		/* main thread returns */
}

void *
thread_main(void *arg)
{
	int				connfd;
	void			web_child(int);
	socklen_t		clilen;
	struct sockaddr	*cliaddr;

	if ((cliaddr = malloc(addrlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	printf("thread %d starting\n", (int) arg);
	for ( ; ; ) {
		clilen = addrlen;
		if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
			perror("accept error");
			exit(1);
		}
		tptr[(int) arg].thread_count++;

		web_child(connfd);		/* process the request */
		if (close(connfd) == -1) {
			perror("close error");
			exit(1);
		}
	}
}
