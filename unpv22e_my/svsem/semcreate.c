#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	SVSEM_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV semaphores */

int
main(int argc, char **argv)
{
	int		c, oflag, semid, nsems;
	key_t	key;

	oflag = SVSEM_MODE | IPC_CREAT;
	while ( (c = getopt(argc, argv, "e")) != -1) {
		switch (c) {
		case 'e':
			oflag |= IPC_EXCL;
			break;
		case '?':
			exit(1);
		}
	}
	if (optind != argc - 2) {
		fprintf(stderr, "usage: semcreate [ -e ] <pathname> <nsems>\n");
		exit(1);
	}
	nsems = atoi(argv[optind + 1]);

	if ((key = ftok(argv[optind], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[optind], 0, strerror(errno));
		exit(1);
	}
	if ((semid = semget(key, nsems, oflag)) == -1) {
		perror("semget error");
		exit(1);
	}
	exit(0);
}
