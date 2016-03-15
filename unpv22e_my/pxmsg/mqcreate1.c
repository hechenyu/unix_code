#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

int main(int argc, char **argv)
{
	int c, flags;
	mqd_t mqd;

	flags = O_RDWR | O_CREAT;
	while ((c = getopt(argc, argv, "e")) != -1) {
		switch (c) {
		case 'e':
			flags |= O_EXCL;
			break;
		case '?':
			exit(1);
		}
	}
	if (optind != argc - 1) {
		fprintf(stderr, "usage: mqcreate [ -e ] <name>\n");
		exit(1);
	}

	/* in linux, look path from /dev/mqueue */
	if ((mqd = mq_open(argv[optind], flags, FILE_MODE, NULL)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", argv[optind], strerror(errno));
		exit(1);
	}

	if (mq_close(mqd) == -1) {
		perror("mq_close error");
		exit(1);
	}
	exit(0);
}
