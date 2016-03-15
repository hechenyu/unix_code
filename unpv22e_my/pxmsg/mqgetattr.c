#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	mqd_t mqd;
	struct mq_attr attr;

	if (argc != 2) {
		fprintf(stderr, "usage: mqgetattr <name>\n");
		exit(1);
	}

	if ((mqd = mq_open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	if (mq_getattr(mqd, &attr) == -1) {
		perror("mq_getattr error");
		exit(1);
	}
	printf("max #msgs = %ld, max #bytes/msg = %ld, "
		   "#currently on queue = %ld\n",
		   attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);

	if (mq_close(mqd) == -1) {
		perror("mq_close error");
		exit(1);
	}
	exit(0);
}
