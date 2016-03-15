#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

volatile sig_atomic_t mqflag;	/* set nonzero by signal handler */
static void sig_usr1(int);

int main(int argc, char **argv)
{
	mqd_t mqd;
	void *buff;
	ssize_t	n;
	sigset_t zeromask, newmask, oldmask;
	struct mq_attr attr;
	struct sigevent	sigev;

	if (argc != 2) {
		fprintf(stderr, "usage: mqnotifysig3 <name>\n");
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

	if (sigemptyset(&zeromask) == -1) {		/* no signals blocked */
		perror("sigemptyset error");
		exit(1);
	}
	if (sigemptyset(&newmask) == -1) {
		perror("sigemptyset error");
		exit(1);
	}
	if (sigemptyset(&oldmask) == -1) {
		perror("sigemptyset error");
		exit(1);
	}
	if (sigaddset(&newmask, SIGUSR1) == -1) {
		perror("sigaddset error");
		exit(1);
	}

	/* 4establish signal handler, enable notification */
	if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {
		perror("singal error");
		exit(1);
	}
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	if (mq_notify(mqd, &sigev) == -1) {
		perror("mq_notify error");
		exit(1);
	}

	for ( ; ; ) {
		if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) == -1) {	/* block SIGUSR1 */
			perror("sigprocmask error");
			exit(1);
		}
			
		while (mqflag == 0)
			sigsuspend(&zeromask);
		mqflag = 0;		/* reset flag */

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
		if (sigprocmask(SIG_UNBLOCK, &newmask, NULL) == -1) {	/* unblock SIGUSR1 */
			perror("sigprocmask error");
			exit(1);
		}
	}
	exit(0);
}

static void sig_usr1(int signo)
{
	mqflag = 1;
	return;
}
