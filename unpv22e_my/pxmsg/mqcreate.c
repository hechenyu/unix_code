#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

struct mq_attr attr;	/* mq_maxmsg and mq_msgsize both init to 0 */

int
main(int argc, char **argv)
{
	int c, flags;
	mqd_t mqd;

	flags = O_RDWR | O_CREAT;
	while ((c = getopt(argc, argv, "em:z:")) != -1) {
		switch (c) {
		case 'e':
			flags |= O_EXCL;
			break;

		case 'm':
			attr.mq_maxmsg = atol(optarg);
			break;

		case 'z':
			attr.mq_msgsize = atol(optarg);
			break;
		case '?':
			exit(1);
		}
	}
	if (optind != argc - 1) {
		fprintf(stderr, "usage: mqcreate [ -e ] [ -m maxmsg -z msgsize ] <name>\n");
		exit(1);
	}

	if ((attr.mq_maxmsg != 0 && attr.mq_msgsize == 0) ||
		(attr.mq_maxmsg == 0 && attr.mq_msgsize != 0)) {
		fprintf(stderr, "must specify both -m maxmsg and -z msgsize");
		exit(1);
	}

	if ((mqd = mq_open(argv[optind], flags, FILE_MODE,
		(attr.mq_maxmsg != 0) ? &attr : NULL)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", argv[optind], strerror(errno));
		exit(1);
	}

	if (mq_close(mqd) == -1) {
		perror("mq_close error");
		exit(1);
	}
	exit(0);
}
