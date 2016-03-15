#include <sys/msg.h>
#include <sys/ipc.h>

#include <stdio.h>
#include <stdlib.h>

//#include "msgbuf.h"	// gcc use -D_GNU_SOURCE instead of 

#define	MAXMSG	(8192 + sizeof(long))

int
main(int argc, char **argv)
{
	int		mqid;
	ssize_t	n;
	struct msgbuf	*buff;

	if (argc != 2) {
		fprintf(stderr, "usage: msgrcvid <mqid>\n");
		exit(1);
	}
	mqid = atoi(argv[1]);

	if ((buff = malloc(MAXMSG)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	if ((n = msgrcv(mqid, buff, MAXMSG, 0, 0)) == -1) {
		perror("msgrcv error");
		exit(1);
	}
	printf("read %d bytes, type = %ld\n", n, buff->mtype);

	exit(0);
}
