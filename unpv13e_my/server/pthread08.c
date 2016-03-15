#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include "pthread08.h"

void
thread_make(int i)
{
	void	*thread_main(void *);
	int 	n;

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
	int		connfd;
	void	web_child(int);
	int		n;

	printf("thread %d starting\n", (int) arg);
	for ( ; ; ) {
    	if ((n = pthread_mutex_lock(&clifd_mutex)) != 0) {
			errno = n; 
			perror("pthread_mutex_lock error");
			exit(1);
		}
		while (iget == iput) {
			if ((n = pthread_cond_wait(&clifd_cond, &clifd_mutex)) != 0) {
				errno = n; 
				perror("pthread_cond_wait error");
				exit(1);
			}
		}
		connfd = clifd[iget];	/* connected socket to service */
		if (++iget == MAXNCLI)
			iget = 0;
		if ((n = pthread_mutex_unlock(&clifd_mutex)) != 0) {
			errno = n; 
			perror("pthread_mutex_unlock error");
			exit(1);
		}
		tptr[(int) arg].thread_count++;

		web_child(connfd);		/* process request */
		if (close(connfd) == -1) {
			perror("close error");
			exit(1);
		}
	}
}
