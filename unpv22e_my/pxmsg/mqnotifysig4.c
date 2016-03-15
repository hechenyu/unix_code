#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int main(int argc, char **argv)
{
	int signo;
	mqd_t mqd;
	void *buff;
	ssize_t	n;
	sigset_t newmask;
	struct mq_attr attr;
	struct sigevent	sigev;
	int err;

	if (argc != 2) {
		fprintf(stderr, "usage: mqnotifysig4 <name>\n");
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

	if (sigemptyset(&newmask) == -1) {
		perror("sigemptyset error");
		exit(1);
	}
	if (sigaddset(&newmask, SIGUSR1) == -1) {
		perror("sigaddset error");
		exit(1);
	}
	if (sigprocmask(SIG_BLOCK, &newmask, NULL) == -1) {	/* block SIGUSR1 */
		perror("sigprocmask error");
		exit(1);
	}

	/* 4establish signal handler, enable notification */
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	if (mq_notify(mqd, &sigev) == -1) {
		perror("mq_notify error");
		exit(1);
	}

	for ( ; ; ) {
		if ((err = sigwait(&newmask, &signo)) != 0) {
			perror("sigwait error");
			exit(1);
		}
		if (signo == SIGUSR1) {
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
