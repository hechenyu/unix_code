#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	int c, flags;
	mqd_t mqd;
	ssize_t n;
	unsigned int prio;
	void *buff;
	struct mq_attr attr;

	flags = O_RDONLY;
	while ((c = getopt(argc, argv, "n")) != -1) {
		switch (c) {
		case 'n':
			flags |= O_NONBLOCK;
			break;
		case '?':
			exit(1);
		}
	}
	if (optind != argc - 1) {
		fprintf(stderr, "usage: mqreceive [ -n ] <name>\n");
		exit(1);
	}

	if ((mqd = mq_open(argv[optind], flags)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", argv[optind], strerror(errno));
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

	if ((n = mq_receive(mqd, buff, attr.mq_msgsize, &prio)) == -1) {
		perror("mq_receive error");
		exit(1);
	}
	printf("read %ld bytes, priority = %u\n", (long) n, prio);

	exit(0);
}
