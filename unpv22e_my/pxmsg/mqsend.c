#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	mqd_t mqd;
	void *ptr;
	size_t len;
	unsigned int prio;

	if (argc != 4) {
		fprintf(stderr, "usage: mqsend <name> <#bytes> <priority>\n");
		exit(1);
	}
	len = atoi(argv[2]);
	prio = atoi(argv[3]);

	if ((mqd = mq_open(argv[1], O_WRONLY)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	if ((ptr = calloc(len, sizeof(char))) == NULL) {
		perror("calloc error");
		exit(1);
	}
	if (mq_send(mqd, ptr, len, prio) == -1) {
		perror("mq_send error");
		exit(1);
	}

	exit(0);
}
