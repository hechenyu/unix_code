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
	int		c, id, oflag;
	char	*ptr;
	size_t	length;
	key_t	key;

	oflag = SVSHM_MODE | IPC_CREAT;
	while ((c = getopt(argc, argv, "e")) != -1) {
		switch (c) {
		case 'e':
			oflag |= IPC_EXCL;
			break;
		case '?':
			exit(1);
		}
	}
	if (optind != argc - 2) {
		fprintf(stderr, "usage: shmget [ -e ] <pathname> <length>\n");
		exit(1);
	}
	length = atoi(argv[optind + 1]);

	if ((key = ftok(argv[optind], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[optind], 0, strerror(errno));
		exit(1);
	}
	if ((id = shmget(key, length, oflag)) == -1) {
		perror("shmget error");
		exit(1);
	}
	if ((ptr = shmat(id, NULL, 0)) == (void *) -1) {
		perror("shmat error");
		exit(1);
	}

	printf("key: %x, %d; id: %x, %d\n", (int) key, (int) key, (int) id, (int) id);

	exit(0);
}
