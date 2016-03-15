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
	key_t	key;

	if (argc != 2) {
		fprintf(stderr, "usage: semgetvalues <pathname>\n");
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

		/* 4allocate memory to hold all the values in the set */
	if ((ptr = calloc(nsems, sizeof(unsigned short))) == NULL) {
		perror("calloc error");
		exit(1);
	}
	arg.array = ptr;

		/* 4fetch the values and print */
	if (semctl(semid, 0, GETALL, arg) == -1) {
		perror("semctl error");
		exit(1);
	}
	for (i = 0; i < nsems; i++)
		printf("semval[%d] = %d\n", i, ptr[i]);

	exit(0);
}
