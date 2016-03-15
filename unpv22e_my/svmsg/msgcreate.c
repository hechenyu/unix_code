#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

int
main(int argc, char **argv)
{
	int		c, oflag, mqid;
	key_t	key;

	oflag = SVMSG_MODE | IPC_CREAT;
	while ((c = getopt(argc, argv, "e")) != -1) {
		switch (c) {
		case 'e':
			oflag |= IPC_EXCL;
			break;
		case '?':
			exit(1);
		}
	}
	if (optind != argc - 1) {
		fprintf(stderr, "usage: msgcreate [ -e ] <pathname>\n");
		exit(1);
	}

	if ((key = ftok(argv[optind], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[optind], 0, strerror(errno));
		exit(1);
	}
	if ((mqid = msgget(key, oflag)) == -1) {
		perror("msgget error");
		exit(1);
	}
	exit(0);
}
