#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char **argv)
{
	int		mqid;
	key_t	key;

	if (argc != 2) {
		fprintf(stderr, "usage: msgrmid <pathname>\n");
		exit(1);
	}

	if ((key = ftok(argv[1], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[1], 0, strerror(errno));
		exit(1);
	}
	if ((mqid = msgget(key, 0)) == -1) {
		perror("msgget error");
		exit(1);
	}
	if (msgctl(mqid, IPC_RMID, NULL) == -1) {
		perror("msgctl error");
		exit(1);
	}

	exit(0);
}
