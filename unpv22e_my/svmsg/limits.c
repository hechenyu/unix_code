#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

#define	MAX_DATA	64*1024
#define	MAX_NMESG	4096
#define	MAX_NIDS	4096
int		max_mesg;

struct mymesg {
  long	type;
  char	data[MAX_DATA];
} mesg;

int
main(int argc, char **argv)
{
	int		i, j, msqid, qid[MAX_NIDS];

		/* 4first try and determine maximum amount of data we can send */
	if ((msqid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) {
		perror("msgget error");
		exit(1);
	}
	mesg.type = 1;
	for (i = MAX_DATA; i > 0; i -= 128) {
		if (msgsnd(msqid, &mesg, i, 0) == 0) {
			printf("maximum amount of data per message = %d\n", i);
			max_mesg = i;
			break;
		}
		if (errno != EINVAL) {
			fprintf(stderr, "msgsnd error for length %d: %s\n", i, strerror(errno));
			exit(1);
		}
	}
	if (i == 0) {
		fprintf(stderr, "i == 0");
		exit(1);
	}
	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		perror("msgctl error");
		exit(1);
	}

		/* 4see how many messages of varying size can be put onto a queue */
	mesg.type = 1;
	for (i = 8; i <= max_mesg; i *= 2) {
		if ((msqid = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) {
			perror("msgget error");
			exit(1);
		}
		for (j = 0; j < MAX_NMESG; j++) {
			if (msgsnd(msqid, &mesg, i, IPC_NOWAIT) != 0) {
				if (errno == EAGAIN)
					break;
				fprintf(stderr, "msgsnd error, i = %d, j = %d: %s\n", i, j, strerror(errno));
				break;
			}
		}
		printf("%d %d-byte messages were placed onto queue,", j, i);
		printf(" %d bytes total\n", i*j);
		if (msgctl(msqid, IPC_RMID, NULL) == -1) {
			perror("msgctl error");
			exit(1);
		}
	}

		/* 4see how many identifiers we can "open" */
	mesg.type = 1;
	for (i = 0; i <= MAX_NIDS; i++) {
		if ( (qid[i] = msgget(IPC_PRIVATE, SVMSG_MODE | IPC_CREAT)) == -1) {
			printf("%d identifiers open at once\n", i);
			break;
		}
	}
	for (j = 0; j < i; j++) {
		if (msgctl(qid[j], IPC_RMID, NULL) == -1) {
			perror("msgctl error");
			exit(1);
		}
	}

	exit(0);
}
