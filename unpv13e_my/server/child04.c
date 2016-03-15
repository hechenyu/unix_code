#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void
my_lock_wait();

void
my_lock_release();

pid_t
child_make(int i, int listenfd, int addrlen)
{
	pid_t	pid;
	void	child_main(int, int, int);

	if ( (pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid > 0) {
		return(pid);		/* parent */
	}

	child_main(i, listenfd, addrlen);	/* never returns */
}

void
child_main(int i, int listenfd, int addrlen)
{
	int				connfd;
	void			web_child(int);
	socklen_t		clilen;
	struct sockaddr	*cliaddr;

	if ((cliaddr = malloc(addrlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	printf("child %ld starting\n", (long) getpid());
	for ( ; ; ) {
		clilen = addrlen;
		my_lock_wait();
		if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
			perror("accept error");
			exit(1);
		}
		my_lock_release();

		web_child(connfd);		/* process the request */
		if (close(connfd) == -1) {
			perror("close error");
			exit(1);
		}
	}
}
