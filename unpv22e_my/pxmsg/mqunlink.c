#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: mqunlink <name>\n");
		exit(1);
	}

	if (mq_unlink(argv[1]) == -1) {
		perror("mq_unlink error");
		exit(1);
	}

	exit(0);
}
