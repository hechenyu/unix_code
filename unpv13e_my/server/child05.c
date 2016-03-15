/* include child_make */
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "child.h"

ssize_t
read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);

pid_t
child_make(int i, int listenfd, int addrlen)
{
	int		sockfd[2];
	pid_t	pid;
	void	child_main(int, int, int);

	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) < 0) {
		perror("socketpair error");
		exit(1);
	}

	if ( (pid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (pid > 0) {
		if (close(sockfd[1]) == -1) {
			perror("close error");
			exit(1);
		}
		cptr[i].child_pid = pid;
		cptr[i].child_pipefd = sockfd[0];
		cptr[i].child_status = 0;
		return(pid);		/* parent */
	}

	if (dup2(sockfd[1], STDERR_FILENO) == -1) {		/* child's stream pipe to parent */
		perror("dup2 error");
		exit(1);
	}
	if (close(sockfd[0]) == -1) {
		perror("close error");
		exit(1);
	}
	if (close(sockfd[1]) == -1) {
		perror("close error");
		exit(1);
	}
	if (close(listenfd) == -1) {					/* child does not need this open */
		perror("close error");
		exit(1);
	}
	child_main(i, listenfd, addrlen);	/* never returns */
}
/* end child_make */

/* include child_main */
void
child_main(int i, int listenfd, int addrlen)
{
	char			c;
	int				connfd;
	ssize_t			n;
	void			web_child(int);

	printf("child %ld starting\n", (long) getpid());
	for ( ; ; ) {
		if ( (n = read_fd(STDERR_FILENO, &c, 1, &connfd)) < 0) {
			perror("read_fd error");
			exit(1);
		} else if (n == 0) {
			fprintf(stderr, "read_fd returned 0\n");
			exit(1);
		}
		if (connfd < 0) {
			fprintf(stderr, "no descriptor from read_fd\n");
			exit(1);
		}

		web_child(connfd);				/* process request */
		if (close(connfd) == -1) {
			perror("close error");
			exit(1);
		}

		if (write(STDERR_FILENO, "", 1) != 1) {	/* tell parent we're ready again */
			perror("write error");
			exit(1);
		}
	}
}
/* end child_main */
