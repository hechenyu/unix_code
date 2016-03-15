#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "semun.h"

#define	SVSEM_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV semaphores */

#define	NSEMS	10		/* #semaphores in set */

int
main(int argc, char **argv)
{
	int		semid, i, j;
	unsigned short	*ptr;
	union semun	arg;

		/* 4allocate memory to hold all the values in the set */
	if ((ptr = calloc(NSEMS, sizeof(unsigned short))) == NULL) {
		perror("calloc error");
		exit(1);
	}
	arg.array = ptr;

	for (j = 0; j < 1000; j++) {
			/* 4create a semaphore set */
		if ((semid = semget(100, NSEMS, IPC_CREAT | IPC_EXCL | SVSEM_MODE)) == -1) {
			perror("semget error");
			exit(1);
		}

			/* 4fetch the "initial" values and print */
		if (semctl(semid, 0, GETALL, arg) == -1) {
			perror("semctl error");
			exit(1);
		}
		for (i = 0; i < NSEMS; i++)
			printf("semval[%d] = %d\n", i, ptr[i]);

			/* 4store new values for entire set */
		for (i = 0; i < NSEMS; i++)
			ptr[i] = rand() % 10;
		if (semctl(semid, 0, SETALL, arg) == -1) {
			perror("semctl error");
			exit(1);
		}

		if (semctl(semid, 0, GETALL, arg) == -1) {
			perror("semctl error");
			exit(1);
		}
		for (i = 0; i < NSEMS; i++)
			printf("semval[%d] = %d\n", i, ptr[i]);

			/* 4and delete the set */
		if (semctl(semid, 0, IPC_RMID) == -1) {
			perror("semctl error");
			exit(1);
		}
	}

	exit(0);
}
