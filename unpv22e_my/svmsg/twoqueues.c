#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

int
main(int argc, char **argv)
{
	int		msqid1, msqid2;

	if ((msqid1 = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) {
		perror("msgget error");
		exit(1);
	}
	if ((msqid2 = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) {
		perror("msgget error");
		exit(1);
	}
	printf("msqid1 = %d, msgqid2 = %d\n", msqid1, msqid2);

	exit(0);
}
