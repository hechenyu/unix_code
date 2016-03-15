#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */
int
main(int argc, char **argv)
{
	int		i, msqid;

	for (i = 0; i < 10; i++) {
		if ((msqid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) {
			perror("msgget error");
			exit(1);
		}
		printf("msqid = %d\n", msqid);

		if (msgctl(msqid, IPC_RMID, NULL) == -1) {
			perror("msgctl error");
			exit(1);
		}
	}
	exit(0);
}
