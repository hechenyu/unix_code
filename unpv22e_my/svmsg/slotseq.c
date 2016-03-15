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
	struct msqid_ds	info;

	for (i = 0; i < 10; i++) {
		if ((msqid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) {
			perror("msgget error");
			exit(1);
		}
		if (msgctl(msqid, IPC_STAT, &info) == -1) {
			perror("msgctl error");
			exit(1);
		}
		printf("msqid = %d, seq = %lu\n", msqid, (unsigned long) info.msg_perm.__seq);

		if (msgctl(msqid, IPC_RMID, NULL) == -1) {
			perror("msgctl error");
			exit(1);
		}
	}
	exit(0);
}
