/* semunlink.c */
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "usage: semunlink <name>\n");
		exit(1);
	}

	if (sem_unlink(argv[1]) == -1) {
		perror("sem_unlink error");
		exit(1);
	}

	exit(0);
}
