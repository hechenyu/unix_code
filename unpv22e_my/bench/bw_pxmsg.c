/* include bw_pxmsg1 */
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <mqueue.h>

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

#define	NAME	"/bw_pxmsg"

void	reader(int, mqd_t, int);
void	writer(int, mqd_t);

void	*buf;
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
	int		i, nloop, contpipe[2];
	mqd_t	mq;
	pid_t	childpid;
	struct mq_attr	attr;
	double st;	// stop time

	if (argc != 4) {
		fprintf(stderr, "usage: bw_pxmsg <#loops> <#mbytes> <#bytes/write>\n");
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

	if (pipe(contpipe) < 0) {
		perror("pipe error");
		exit(1);
	}
	mq_unlink(NAME);	/* error OK */
	attr.mq_maxmsg = 4;
	attr.mq_msgsize = xfersize;
	if ((mq = mq_open(NAME, O_RDWR | O_CREAT, FILE_MODE, &attr)) == -1) {
		fprintf(stderr, "mq_open error for %s: %s\n", NAME, strerror(errno));
		exit(1);
	}

	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		writer(contpipe[0], mq);		/* child */
		exit(0);
	}
		/* 4parent */
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	for (i = 0; i < nloop; i++)
		reader(contpipe[1], mq, totalnbytes);
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("bandwidth: %.3f MB/sec\n",
		   totalnbytes / st * nloop);

	kill(childpid, SIGTERM);
	if (mq_close(mq) == -1) {
		perror("mq_close error");
		exit(1);
	}
	if (mq_unlink(NAME) == -1) {
		perror("mq_unlink error");
		exit(1);
	}
	exit(0);
}
/* end bw_pxmsg1 */

/* include bw_pxmsg2 */
void
writer(int contfd, mqd_t mqsend)
{
	int		ntowrite;

	for ( ; ; ) {
		if (read(contfd, &ntowrite, sizeof(ntowrite)) == -1) {
			perror("read error");
			exit(1);
		}

		while (ntowrite > 0) {
			if (mq_send(mqsend, buf, xfersize, 0) == -1) {
				perror("mq_send error");
				exit(1);
			}
			ntowrite -= xfersize;
		}
	}
}

void
reader(int contfd, mqd_t mqrecv, int nbytes)
{
	ssize_t	n;

	if (write(contfd, &nbytes, sizeof(nbytes)) != sizeof(nbytes)) {
		perror("write error");
		exit(1);
	}

	while ((nbytes > 0) &&
		   ( (n = mq_receive(mqrecv, buf, xfersize, NULL)) > 0)) {
		nbytes -= n;
	}
	if (n == -1) {
		perror("mq_receive error");
		exit(1);
	}
}
/* end bw_pxmsg2 */
