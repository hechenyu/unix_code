/* include lat_pipe1 */
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int
start_time(void);

double
stop_time(void);

void
doit(int readfd, int writefd)
{
	char	c;

	if (write(writefd, &c, 1) != 1) {
		perror("write error");
		exit(1);
	}
	if (read(readfd, &c, 1) != 1) {
		perror("read error");
		exit(1);
	}
}

int
main(int argc, char **argv)
{
	int		i, nloop, pipe1[2], pipe2[2];
	char	c;
	pid_t	childpid;
	double st;	// stop time

	if (argc != 2) {
		fprintf(stderr, "usage: lat_pipe <#loops>\n");
		exit(1);
	}
	nloop = atoi(argv[1]);

	if (pipe(pipe1) < 0) {
		perror("pipe error");
		exit(1);
	}
	if (pipe(pipe2) < 0) {
		perror("pipe error");
		exit(1);
	}

	if ((childpid = fork()) == -1) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		for ( ; ; ) {		/* child */
			if (read(pipe1[0], &c, 1) != 1) {
				perror("read error");
				exit(1);
			}
		    if (write(pipe2[1], &c, 1) != 1) {
				perror("write error");
				exit(1);
			}
		}
		exit(0);
	}
		/* 4parent */
	doit(pipe2[0], pipe1[1]);

	if (start_time() == -1) {
		perror("start_time error");
		exit(1);
	}
	for (i = 0; i < nloop; i++)
		doit(pipe2[0], pipe1[1]);
	if ((st = stop_time()) == 0.0) {
		perror("stop_time error");
		exit(1);
	}
	printf("latency: %.3f usec\n", st / nloop);

	if (kill(childpid, SIGTERM) == -1) {
		perror("kill error");
		exit(1);
	}
	exit(0);
}
/* end lat_pipe1 */
