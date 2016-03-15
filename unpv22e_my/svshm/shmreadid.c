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

	if (argc != 2) {
		fprintf(stderr, "usage: shmread <id>\n");
		exit(1);
	}

	id = atol(argv[1]);
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
