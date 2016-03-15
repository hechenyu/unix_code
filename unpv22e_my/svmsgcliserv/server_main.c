#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include "svmsg.h"

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

void	server(int, int);

int
main(int argc, char **argv)
{
	int		readid, writeid;

	if ((readid = msgget(MQ_KEY1, SVMSG_MODE | IPC_CREAT)) == -1) {
		perror("msgget error");
		exit(1);
	}
	if ((writeid = msgget(MQ_KEY2, SVMSG_MODE | IPC_CREAT)) == -1) {
		perror("msgget error");
		exit(1);
	}

	server(readid, writeid);

	exit(0);
}
