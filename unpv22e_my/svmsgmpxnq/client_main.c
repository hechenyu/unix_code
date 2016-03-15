#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include	"svmsg.h"

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

void	client(int, int);

int
main(int argc, char **argv)
{
	int		readid, writeid;

		/* 4server must create its well-known queue */
	if ((writeid = msgget(MQ_KEY1, 0)) == -1) {
		perror("msgget error");
		exit(1);
	}
		/* 4we create our own private queue */
	if ((readid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) {
		perror("msgget error");
		exit(1);
	}

	client(readid, writeid);

		/* 4and delete our private queue */
	if (msgctl(readid, IPC_RMID, NULL) == -1) {
		perror("msgctl error");
		exit(1);
	}

	exit(0);
}
