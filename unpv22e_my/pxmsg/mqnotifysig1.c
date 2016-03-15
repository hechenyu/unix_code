#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

mqd_t mqd;
void *buff;
struct mq_attr attr;
struct sigevent sigev;

static void sig_usr1(int);

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: mqnotifysig1 <name>\n");
		exit(1);
	}

	/* 4open queue, get attributes, allocate read buffer */
	if ((mqd = mq_open(argv[1], O_RDONLY)) == -1) {
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

	for ( ; ; )
		pause();		/* signal handler does everything */
	exit(0);
}

static void sig_usr1(int signo)
{
	ssize_t	n;

	if (mq_notify(mqd, &sigev) == -1) {	/* reregister first */
		perror("mq_notify error");
		exit(1);
	}
	if ((n = mq_receive(mqd, buff, attr.mq_msgsize, NULL)) == -1) {
		perror("mq_receive error");
		exit(1);
	}
	printf("SIGUSR1 received, read %ld bytes\n", (long) n);
	return;
}
