/* include bw_pipe1 */
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void	reader(int, int, int);
void	writer(int, int);

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
	int		i, nloop, contpipe[2], datapipe[2];
	pid_t	childpid;
	double st;	// stop time

	if (argc != 4) {
		fprintf(stderr, "usage: bw_pipe <#loops> <#mbytes> <#bytes/write>\n");
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
	if (pipe(datapipe) < 0) {
		perror("pipe error");
		exit(1);
	}

	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		writer(contpipe[0], datapipe[1]);	/* child */
		exit(0);
	}
		/* 4parent */
	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	for (i = 0; i < nloop; i++)
		reader(contpipe[1], datapipe[0], totalnbytes);
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("bandwidth: %.3f MB/sec\n",
		   totalnbytes / st * nloop);
	kill(childpid, SIGTERM);
	exit(0);
}
/* end bw_pipe1 */

/* include bw_pipe2 */
void
writer(int contfd, int datafd)
{
	int		ntowrite;

	for ( ; ; ) {
		if (read(contfd, &ntowrite, sizeof(ntowrite)) == -1) {
			perror("read error");
			exit(1);
		}

		while (ntowrite > 0) {
			if (write(datafd, buf, xfersize) != xfersize) {
				perror("write error");
				exit(1);
			}
			ntowrite -= xfersize;
		}
	}
}

void
reader(int contfd, int datafd, int nbytes)
{
	ssize_t	n;

	if (write(contfd, &nbytes, sizeof(nbytes)) != sizeof(nbytes)) { 
		perror("write error");
		exit(1);
	}

	while ((nbytes > 0) &&
		   ( (n = read(datafd, buf, xfersize)) > 0)) {
		nbytes -= n;
	}
	if (n == -1) {
		perror("read error");
		exit(1);
	}
}
/* end bw_pipe2 */
