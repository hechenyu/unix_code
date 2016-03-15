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
	int		i, id;
	struct shmid_ds	buff;
	unsigned char	c, *ptr;
	key_t	key;

	if (argc != 2) {
		fprintf(stderr, "usage: shmread <pathname>\n");
		exit(1);
	}

	if ((key = ftok(argv[1], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[1], 0, strerror(errno));
		exit(1);
	}
	if ((id = shmget(key, 0, SVSHM_MODE)) == -1) {
		perror("shmget error");
		exit(1);
	}
	if ((ptr = shmat(id, NULL, 0)) == (void *) -1) {
		perror("shmat error");
		exit(1);
	}
	if (shmctl(id, IPC_STAT, &buff) == -1) {
		perror("shmctl error");
		exit(1);
	}

		/* 4check that ptr[0] = 0, ptr[1] = 1, etc. */
	for (i = 0; i < buff.shm_segsz; i++)
		if ( (c = *ptr++) != (i % 256)) {
			fprintf(stderr, "ptr[%d] = %d\n", i, c);
		}

	exit(0);
}
