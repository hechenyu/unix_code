#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	SVSHM_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV shared memory */

int
main(int argc, char **argv)
{
	int		id;
	key_t	key;

	if (argc != 2) {
		fprintf(stderr, "usage: shmrmid <pathname>\n");
		exit(1);
	}

	if ((key = ftok(argv[optind], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[optind], 0, strerror(errno));
		exit(1);
	}
	if ((id = shmget(key, 0, SVSHM_MODE)) == -1) {
		perror("shmget error");
		exit(1);
	}
	if (shmctl(id, IPC_RMID, NULL) == -1) {
		perror("shmctl error");
		exit(1);
	}

	exit(0);
}
