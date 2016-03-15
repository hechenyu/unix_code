#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//#include "msgbuf.h"	// gcc use -D_GNU_SOURCE instead of 

#define MSG_R	S_IRUSR 
					/* default permissions for new SV message queues */

#define	MAXMSG	(8192 + sizeof(long))

int
main(int argc, char **argv)
{
	int		c, flag, mqid;
	long	type;
	ssize_t	n;
	struct msgbuf	*buff;
	key_t	key;

	type = flag = 0;
	while ((c = getopt(argc, argv, "nt:")) != -1) {
		switch (c) {
		case 'n':
			flag |= IPC_NOWAIT;
			break;
		case 't':
			type = atol(optarg);
			break;
		case '?':
			exit(1);
		}
	}
	if (optind != argc - 1) {
		fprintf(stderr, "usage: msgrcv [ -n ] [ -t type ] <pathname>\n");
		exit(1);
	}

	if ((key = ftok(argv[optind], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[optind], 0, strerror(errno));
		exit(1);
	}
	if ((mqid = msgget(key, MSG_R)) == -1) {
		perror("msgget error");
		exit(1);
	}

	if ((buff = malloc(MAXMSG)) == NULL) {
		perror("malloc error");
		exit(1);
	}

	if ((n = msgrcv(mqid, buff, MAXMSG, type, flag)) == -1) {
		perror("msgrcv error");
		exit(1);
	}
	printf("read %d bytes, type = %ld\n", n, buff->mtype);

	exit(0);
}
