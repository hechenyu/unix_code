#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//#include "msgbuf.h"	// gcc use -D_GNU_SOURCE instead of 

#define MSG_W	S_IWUSR 
					/* default permissions for new SV message queues */

int
main(int argc, char **argv)
{
	int		mqid;
	size_t	len;
	long	type;
	struct msgbuf	*ptr;
	key_t	key;

	if (argc != 4) {
		fprintf(stderr, "usage: msgsnd <pathname> <#bytes> <type>\n");
		exit(1);
	}
	len = atoi(argv[2]);
	type = atoi(argv[3]);

	if ((key = ftok(argv[1], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[1], 0, strerror(errno));
		exit(1);
	}
	if ((mqid = msgget(key, MSG_W)) == -1) {
		perror("msgget error");
		exit(1);
	}

	if ((ptr = calloc(sizeof(long) + len, sizeof(char))) == NULL) {
		perror("calloc error");
		exit(1);
	}
	ptr->mtype = type;

	if (msgsnd(mqid, ptr, len, 0) == -1) {
		perror("msgsnd error");
		exit(1);
	}

	exit(0);
}
