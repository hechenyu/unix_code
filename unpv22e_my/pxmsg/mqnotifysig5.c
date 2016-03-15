#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int pipefd[2];
static void sig_usr1(int);

/* $$.bp$$ */
int main(int argc, char **argv)
{
	int nfds;
	char c;
	fd_set rset;
	mqd_t mqd;
	void *buff;
	ssize_t	n;
	struct mq_attr attr;
	struct sigevent	sigev;

	if (argc != 2) {
		fprintf(stderr, "usage: mqnotifysig5 <name>\n");
		exit(1);
	}

	/* 4open queue, get attributes, allocate read buffer */
	if ((mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if (mq_getattr(mqd, &attr) == -1) {
		perror("mq_getattr error");
		exit(1);
	}
	if ((buff = malloc(attr.mq_msgsize)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	if (pipe(pipefd) < 0) {
		perror("pipe error");
		exit(1);
	}

	/* 4establish signal handler, enable notification */
	if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	if (mq_notify(mqd, &sigev) == -1) {
		perror("mq_notify error");
		exit(1);
	}

	FD_ZERO(&rset);
	for ( ; ; ) {
		FD_SET(pipefd[0], &rset);
again:
		if ((nfds = select(pipefd[0] + 1, &rset, NULL, NULL, NULL)) < 0) {
			if (errno == EINTR)
				goto again;
			else {
				perror("select error");
				exit(1);
			}
		}

		if (FD_ISSET(pipefd[0], &rset)) {
			if (read(pipefd[0], &c, 1) == -1) {
				perror("read error");
				exit(1);
			}
			if (mq_notify(mqd, &sigev) == -1) {		/* reregister first */
				perror("mq_notify error");
				exit(1);
			}
			while ((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) >= 0) {
				printf("read %ld bytes\n", (long) n);
			}
			if (errno != EAGAIN) {
				perror("mq_receive error");
				exit(1);
			}
		}
	}
	exit(0);
}

static void sig_usr1(int signo)
{
	if (write(pipefd[1], "", 1) != 1) {	/* one byte of 0 */
		perror("write error");
		exit(1);
	}
	return;
}
