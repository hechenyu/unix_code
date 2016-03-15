#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "semun.h"

int
main(int argc, char **argv)
{
	int		semid, nsems, i;
	struct semid_ds	seminfo;
	unsigned short	*ptr;
	union semun	arg;
	key_t key;

	if (argc < 2) {
		fprintf(stderr, "usage: semsetvalues <pathname> [ values ... ]\n");
		exit(1);
	}

		/* 4first get the number of semaphores in the set */
	if ((key = ftok(argv[1], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[1], 0, strerror(errno));
		exit(1);
	}
	if ((semid = semget(key, 0, 0)) == -1) {
		perror("semget error");
		exit(1);
	}
	arg.buf = &seminfo;
	if (semctl(semid, 0, IPC_STAT, arg) == -1) {
		perror("semctl error");
		exit(1);
	}
	nsems = arg.buf->sem_nsems;

		/* 4now get the values from the command line */
	if (argc != nsems + 2) {
		fprintf(stderr, "%d semaphores in set, %d values specified", nsems, argc-2);
		exit(1);
	}

		/* 4allocate memory to hold all the values in the set, and store */
	if ((ptr = calloc(nsems, sizeof(unsigned short))) == NULL) {
		perror("calloc error");
		exit(1);
	}
	arg.array = ptr;
	for (i = 0; i < nsems; i++)
		ptr[i] = atoi(argv[i + 2]);
	if (semctl(semid, 0, SETALL, arg) == -1) {
		perror("semctl error");
		exit(1);
	}

	exit(0);
}
