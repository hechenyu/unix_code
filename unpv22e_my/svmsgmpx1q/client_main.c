#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svmsg.h"

void	client(int, int);

int
main(int argc, char **argv)
{
	int		msqid;

		/* 4server must create the queue */
	if ((msqid = msgget(MQ_KEY1, 0)) == -1) {
		perror("msgget error");
		exit(1);
	}

	client(msqid, msqid);	/* same queue for both directions */

	exit(0);
}
