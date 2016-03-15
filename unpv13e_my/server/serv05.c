/* include serv05a */
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include	"child.h"

#define	max(a, b) ((a) < (b) ? (b) : (a))

int
tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

ssize_t
write_fd(int fd, void *ptr, size_t nbytes, int sendfd);

static int		nchildren;

int
main(int argc, char **argv)
{
	int			listenfd, i, navail, maxfd, nsel, connfd, rc;
	void		sig_int(int);
	pid_t		child_make(int, int, int);
	ssize_t		n;
	fd_set		rset, masterset;
	socklen_t	addrlen, clilen;
	struct sockaddr	*cliaddr;

	if (argc == 3) {
		listenfd = tcp_listen(NULL, argv[1], &addrlen);
	} else if (argc == 4) {
		listenfd = tcp_listen(argv[1], argv[2], &addrlen);
	} else {
		fprintf(stderr, "usage: serv05 [ <host> ] <port#> <#children>\n");
		exit(1);
	}

	FD_ZERO(&masterset);
	FD_SET(listenfd, &masterset);
	maxfd = listenfd;
	if ((cliaddr = malloc(addrlen)) != NULL) {
		perror("malloc error");
		exit(1);
	}

	nchildren = atoi(argv[argc-1]);
	navail = nchildren;
	if ((cptr = calloc(nchildren, sizeof(Child))) == NULL) {
		perror("calloc error");
		exit(1);
	}

		/* 4prefork all the children */
	for (i = 0; i < nchildren; i++) {
		child_make(i, listenfd, addrlen);	/* parent returns */
		FD_SET(cptr[i].child_pipefd, &masterset);
		maxfd = max(maxfd, cptr[i].child_pipefd);
	}

	if (signal(SIGINT, sig_int) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}

	for ( ; ; ) {
		rset = masterset;
		if (navail <= 0)
			FD_CLR(listenfd, &rset);	/* turn off if no available children */
		if ((nsel = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0) {
			perror("select error");
			exit(1);
		}

			/* 4check for new connections */
		if (FD_ISSET(listenfd, &rset)) {
			clilen = addrlen;
			if ((connfd = accept(listenfd, cliaddr, &clilen)) < 0) {
				perror("accept error");
				exit(1);
			}

			for (i = 0; i < nchildren; i++)
				if (cptr[i].child_status == 0)
					break;				/* available */

			if (i == nchildren) {
				fprintf(stderr, "no available children\n");
				exit(1);
			}
			cptr[i].child_status = 1;	/* mark child as busy */
			cptr[i].child_count++;
			navail--;

			if ((n = write_fd(cptr[i].child_pipefd, "", 1, connfd)) < 0) {
				perror("write_fd error");
				exit(1);
			}
			if (close(connfd) == -1) {
				perror("close error");
				exit(1);
			}
			if (--nsel == 0)
				continue;	/* all done with select() results */
		}

			/* 4find any newly-available children */
		for (i = 0; i < nchildren; i++) {
			if (FD_ISSET(cptr[i].child_pipefd, &rset)) {
				if ( (n = read(cptr[i].child_pipefd, &rc, 1)) < 0) {
					perror("read error");
					exit(1);
				} else if (n == 0) {
					fprintf(stderr, "child %d terminated unexpectedly\n", i);
					exit(1);
				}
				cptr[i].child_status = 0;
				navail++;
				if (--nsel == 0)
					break;	/* all done with select() results */
			}
		}
	}
}
/* end serv05a */

void
sig_int(int signo)
{
	int		i;
	void	pr_cpu_time(void);

		/* 4terminate all children */
	for (i = 0; i < nchildren; i++)
		kill(cptr[i].child_pid, SIGTERM);
	while (wait(NULL) > 0)		/* wait for all children */
		;
	if (errno != ECHILD) {
		perror("wait error");
		exit(1);
	}

	pr_cpu_time();

	for (i = 0; i < nchildren; i++)
		printf("child %d, %ld connections\n", i, cptr[i].child_count);

	exit(0);
}
