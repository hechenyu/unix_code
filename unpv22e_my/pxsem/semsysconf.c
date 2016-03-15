#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	long sem_nsems_max, sem_value_max;
	if ((sem_nsems_max = sysconf(_SC_SEM_NSEMS_MAX)) == -1) {
		if (errno != 0)
			perror("sysconf error");
		else
			fprintf(stderr, "sysconf %d not defined\n", _SC_SEM_NSEMS_MAX);
		exit(1);
	}
	if ((sem_value_max = sysconf(_SC_SEM_VALUE_MAX)) == -1) {
		if (errno != 0)
			perror("sysconf error");
		else
			fprintf(stderr, "sysconf %d not defined\n", _SC_SEM_VALUE_MAX);
		exit(1);
	}
	printf("SEM_NSEMS_MAX = %ld, SEM_VALUE_MAX = %ld\n",
		sem_nsems_max, sem_value_max);
	exit(0);
}
