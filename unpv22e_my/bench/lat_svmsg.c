/* include lat_svmsg1 */
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

/* *INDENT-OFF* */
struct msgbuf	p2child = { 1, { 0 } };		/* type = 1 */
struct msgbuf	child2p = { 2, { 0 } };		/* type = 2 */
struct msgbuf	inbuf;
/* *INDENT-ON* */

int
start_time(void);

double
stop_time(void);

void
doit(int msgid)
{
	if (msgsnd(msgid, &p2child, 0, 0) == -1) {
		perror("msgsnd error");
		exit(1);
	}
	if (msgrcv(msgid, &inbuf, sizeof(inbuf.mtext), 2, 0) != 0) {
		perror("msgrcv error");
		exit(1);
	}
}

int
main(int argc, char **argv)
{
	int		i, nloop, msgid;
	pid_t	childpid;
	double st;	// stop time

	if (argc != 2) {
		fprintf(stderr, "usage: lat_svmsg <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);

	if ((msgid = msgget(IPC_PRIVATE, IPC_CREAT | SVMSG_MODE)) == -1) {
		perror("msgget error");
		exit(1);
	}

	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		for ( ; ; ) {		/* child */
			if (msgrcv(msgid, &inbuf, sizeof(inbuf.mtext), 1, 0) != 0) {
				perror("msgrcv error");
				exit(1);
			}
		    if (msgsnd(msgid, &child2p, 0, 0) == -1) {
				perror("msgsnd error");
				exit(1);
			}
		}
		exit(0);
	}
		/* 4parent */
	doit(msgid);

	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	for (i = 0; i < nloop; i++)
		doit(msgid);
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("latency: %.3f usec\n", st / nloop);

	if (kill(childpid, SIGTERM) == -1) {
		perror("kill error");
		exit(1);
	}
	if (msgctl(msgid, IPC_RMID, NULL) == -1) {
		perror("msgctl error");
		exit(1);
	}
	exit(0);
}
/* end lat_svmsg1 */
