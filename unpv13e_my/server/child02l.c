#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
	fd_set			rset;
	socklen_t		clilen;
	struct sockaddr	*cliaddr;

	if ((cliaddr = malloc(addrlen)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	printf("child %ld starting\n", (long) getpid());
	FD_ZERO(&rset);
	for ( ; ; ) {
		FD_SET(listenfd, &rset);
		if (select(listenfd+1, &rset, NULL, NULL, NULL) < 0) {
			perror("select error");
			exit(1);
		}
		if (FD_ISSET(listenfd, &rset) == 0) {
			fprintf(stderr, "listenfd readable\n");
			exit(1);
		}

		clilen = addrlen;
		if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
			perror("accept error");
			exit(1);
		}

		web_child(connfd);		/* process the request */
		if (close(connfd) == -1) {
			perror("close error");
			exit(1);
		}
	}
}
