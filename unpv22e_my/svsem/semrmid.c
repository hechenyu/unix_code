#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char **argv)
{
	int		semid;
	key_t	key;

	if (argc != 2) {
		fprintf(stderr, "usage: semrmid <pathname>\n");
		exit(1);
	}

	if ((key = ftok(argv[1], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[1], 0, strerror(errno));
		exit(1);
	}
	if ((semid = semget(key, 0, 0)) == -1) {
		perror("semget error");
		exit(1);
	}
	if (semctl(semid, 0, IPC_RMID) == -1) {
		perror("semctl error");
		exit(1);
	}

	exit(0);
}
