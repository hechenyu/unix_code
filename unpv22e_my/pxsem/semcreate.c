/* semcreate.c */
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int main(int argc, char **argv)
{
	int c, flags;
	sem_t *sem;
	unsigned int value;

	flags = O_RDWR | O_CREAT;
	value = 1;
	while ((c = getopt(argc, argv, "ei:")) != -1) {
		switch (c) {
		case 'e':
			flags |= O_EXCL;
			break;
		case 'i':
			value = atoi(optarg);
			break;
		case '?':
			exit(1);
		}
	}

	if (optind != argc - 1) {
		fprintf(stderr, "usage: semcreate [ -e ] [ -i initalvalue ] <name>\n");
		exit(1);
	}

	sem = sem_open(argv[optind], flags, FILE_MODE, value);
	if (sem == SEM_FAILED) {
		fprintf(stderr, "sem_open error for %s: %s\n", argv[optind], strerror(errno));
		exit(1);
	}

	if (sem_close(sem) == -1) {
		perror("sem_close error");
		exit(1);
	}

	exit(0);
}
