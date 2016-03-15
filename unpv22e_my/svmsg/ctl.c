#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "msgbuf.h"	// gcc use -D_GNU_SOURCE instead of 

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */
typedef unsigned long ulong_t;

int
main(int argc, char **argv)
{
	int				msqid;
	struct msqid_ds	info;
	struct msgbuf	buf;

	if ((msqid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) {
		perror("msgget error");
		exit(1);
	}

	buf.mtype = 1;
	buf.mtext[0] = 1;
	if (msgsnd(msqid, &buf, 1, 0) == -1) {
		perror("msgsnd error");
		exit(1);
	}

	if (msgctl(msqid, IPC_STAT, &info) == -1) {
		perror("msgctl error");
		exit(1);
	}
	printf("read-write: %03o, cbytes = %lu, qnum = %lu, qbytes = %lu\n",
		   info.msg_perm.mode & 0777, (ulong_t) info.msg_cbytes,
		   (ulong_t) info.msg_qnum, (ulong_t) info.msg_qbytes);

	system("ipcs -q");

	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		perror("msgctl error");
		exit(1);
	}
	exit(0);
}
