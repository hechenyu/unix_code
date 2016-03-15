/* include lat_pxmsg1 */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <mqueue.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define	NAME1	"/lat_pxmsg1"
#define	NAME2	"/lat_pxmsg2"
#define	MAXMSG	   4		/* room for 4096 bytes on queue */
#define	MSGSIZE	1024

int
start_time(void);

double
stop_time(void);

void
doit(mqd_t mqsend, mqd_t mqrecv)
{
	char	buff[MSGSIZE];

	if (mq_send(mqsend, buff, 1, 0) == -1) {
		perror("mq_send error");
		exit(1);
	}
	if (mq_receive(mqrecv, buff, MSGSIZE, NULL) != 1) {
		perror("mq_receive error");
		exit(1);
	}
}

int
main(int argc, char **argv)
{
	int		i, nloop;
	mqd_t	mq1, mq2;
	char	buff[MSGSIZE];
	pid_t	childpid;
	struct mq_attr	attr;
	double st;	// stop time

	if (argc != 2) {
		fprintf(stderr, "usage: lat_pxmsg <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);

	attr.mq_maxmsg = MAXMSG;
	attr.mq_msgsize = MSGSIZE;
	if ((mq1 = mq_open(NAME1, O_RDWR | O_CREAT, FILE_MODE, &attr)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", NAME1, strerror(errno));
		exit(1);
	}
	if ((mq2 = mq_open(NAME2, O_RDWR | O_CREAT, FILE_MODE, &attr)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", NAME1, strerror(errno));
		exit(1);
	}

	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		for ( ; ; ) {			/* child */
			if (mq_receive(mq1, buff, MSGSIZE, NULL) != 1) {
				perror("mq_receive error");
				exit(1);
			}
		    if (mq_send(mq2, buff, 1, 0) == -1) {
				perror("mq_send error");
				exit(1);
			}
		}
		exit(0);
	}
		/* 4parent */
	doit(mq1, mq2);

	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	for (i = 0; i < nloop; i++)
		doit(mq1, mq2);
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("latency: %.3f usec\n", st / nloop);

	if (kill(childpid, SIGTERM) == -1) {
		perror("kill error");
		exit(1);
	}
	if (mq_close(mq1) == -1) {
		perror("mq_close error");
		exit(1);
	}
	if (mq_close(mq2) == -1) {
		perror("mq_close error");
		exit(1);
	}
	if (mq_unlink(NAME1) == -1) {
		perror("mq_unlink error");
		exit(1);
	}
	if (mq_unlink(NAME2) == -1) {
		perror("mq_unlink error");
		exit(1);
	}
	exit(0);
}
/* end lat_pxmsg1 */
