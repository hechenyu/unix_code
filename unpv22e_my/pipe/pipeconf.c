#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: pipeconf <pathname>\n");
		exit(1);
	}

	long pipe_buf, open_max;
	if ((pipe_buf = pathconf(argv[1], _PC_PIPE_BUF)) == -1) {
		if (errno != 0)
			fprintf(stderr, "pathconf error: %s\n", strerror(errno));
		else
			fprintf(stderr, "pathconf %d not defined\n", _PC_PIPE_BUF);
		exit(1);
	}
	if ((open_max = sysconf(_SC_OPEN_MAX)) == -1) {
		if (errno != 0)
			perror("sysconf error");
		else
			fprintf(stderr, "sysconf %d not defined\n", _SC_OPEN_MAX);
		exit(1);
	}
	printf("PIPE_BUF = %ld, OPEN_MAX = %ld\n", pipe_buf, open_max);
	exit(0);
}

