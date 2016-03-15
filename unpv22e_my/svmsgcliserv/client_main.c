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
	int		readid, writeid;

		/* 4assumes server has created the queues */
	if ((writeid = msgget(MQ_KEY1, 0)) == -1) {
		perror("msgget error");
		exit(1);
	}
	if ((readid = msgget(MQ_KEY2, 0)) == -1) {
		perror("msgget error");
		exit(1);
	}

	client(readid, writeid);

		/* 4now we can delete the queues */
	if (msgctl(readid, IPC_RMID, NULL) == -1) {
		perror("msgctl error");
		exit(1);
	}
	if (msgctl(writeid, IPC_RMID, NULL) == -1) {
		perror("msgctl error");
		exit(1);
	}

	exit(0);
}
