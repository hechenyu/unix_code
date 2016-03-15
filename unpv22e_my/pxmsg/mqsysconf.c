#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	long mq_open_max, mq_prio_max;
	if ((mq_open_max = sysconf(_SC_MQ_OPEN_MAX)) == -1) {
		if (errno != 0)
			perror("sysconf error");
		else
			fprintf(stderr, "sysconf %d not defined\n", _SC_MQ_OPEN_MAX);
		exit(1);
	}
	if ((mq_prio_max = sysconf(_SC_MQ_PRIO_MAX)) == -1) {
		if (errno != 0)
			perror("sysconf error");
		else
			fprintf(stderr, "sysconf %d not defined\n", _SC_MQ_PRIO_MAX);
		exit(1);
	}
	printf("MQ_OPEN_MAX = %ld, MQ_PRIO_MAX = %ld\n", mq_open_max, mq_prio_max);
	exit(0);
}
