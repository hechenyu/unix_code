/* include bw_svmsg1 */
#include <sys/msg.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

void	reader(int, int, int);
void	writer(int, int);

struct msgbuf	*buf;
int		totalnbytes, xfersize;

int
start_time(void);

double
stop_time(void);

int
touch(void *vptr, int nbytes);

int
main(int argc, char **argv)
{
	int		i, nloop, contpipe[2], msqid;
	pid_t	childpid;
	double st;	// stop time

	if (argc != 4) {
		fprintf(stderr, "usage: bw_svmsg <#loops> <#mbytes> <#bytes/write>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);
	totalnbytes = atoi(argv[2]) * 1024 * 1024;
	xfersize = atoi(argv[3]);

	if ((buf = valloc(xfersize)) == NULL) {
		perror("valloc error");
		exit(1);
	}
	if (touch(buf, xfersize) == -1) {
		perror("touch error");
		exit(1);
	}
	buf->mtype = 1;

	if (pipe(contpipe) < 0) {
		perror("pipe error");
		exit(1);
	}
	if ((msqid = msgget(IPC_PRIVATE, IPC_CREAT | SVMSG_MODE)) == -1) {
		perror("msgget error");
		exit(1);
	}

	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		writer(contpipe[0], msqid);		/* child */
		exit(0);
	}
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	for (i = 0; i < nloop; i++)
		reader(contpipe[1], msqid, totalnbytes);
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("bandwidth: %.3f MB/sec\n",
		   totalnbytes / st * nloop);

	kill(childpid, SIGTERM);
	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		perror("msgctl error");
		exit(1);
	}
	exit(0);
}
/* end bw_svmsg1 */

/* include bw_svmsg2 */
void
writer(int contfd, int msqid)
{
	int		ntowrite;

	for ( ; ; ) {
		if (read(contfd, &ntowrite, sizeof(ntowrite)) == -1) {
			perror("read error");
			exit(1);
		}

		while (ntowrite > 0) {
			if (msgsnd(msqid, buf, xfersize - sizeof(long), 0) == -1) {
				perror("msgsnd error");
				exit(1);
			}
			ntowrite -= xfersize;
		}
	}
}

void
reader(int contfd, int msqid, int nbytes)
{
	ssize_t	n;

	if (write(contfd, &nbytes, sizeof(nbytes)) != sizeof(nbytes)) { 
		perror("write error");
		exit(1);
	}

	while ((nbytes > 0) &&
		   ( (n = msgrcv(msqid, buf, xfersize - sizeof(long), 0, 0)) > 0)) {
		nbytes -= n + sizeof(long);
	}
	if (n == -1) {
		perror("msgrcv error");
		exit(1);
	}
}
/* end bw_svmsg2 */
