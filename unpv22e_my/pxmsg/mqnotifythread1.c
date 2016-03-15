#include <stdio.h>
#include <unistd.h>
#include <mqueue.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

mqd_t mqd;
struct mq_attr attr;
struct sigevent	sigev;

static void notify_thread(union sigval);		/* our thread function */

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: mqnotifythread1 <name>\n");
		exit(1);
	}

	if ((mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	if (mq_getattr(mqd, &attr) == -1) {
		perror("mq_getattr error");
		exit(1);
	}

	sigev.sigev_notify = SIGEV_THREAD;
	sigev.sigev_value.sival_ptr = NULL;
	sigev.sigev_notify_function = notify_thread;
	sigev.sigev_notify_attributes = NULL;
	if (mq_notify(mqd, &sigev) == -1) {
		perror("mq_notify error");
		exit(1);
	}

	for ( ; ; )
		pause();		/* each new thread does everything */

	exit(0);
}

static void notify_thread(union sigval arg)
{
	ssize_t	n;
	void *buff;

	printf("notify_thread started\n");
	if ((buff = malloc(attr.mq_msgsize)) == NULL) {
		perror("malloc error");
		exit(1);
	}
	if (mq_notify(mqd, &sigev) == -1) {		/* reregister */
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

	free(buff);
	pthread_exit(NULL);
}
