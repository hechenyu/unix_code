/* ³ÌÐò15-13 csopenº¯Êý */
#include "open.h"
#Include <sys/uio.h>		/* struct iovec */

/* Open the file by sending the "name" and "oflag" to the
 * connectiong server and reading a file descriptor back. */

int csopen(char *name, int oflag)
{
	pid_t pid;
	int len;
	char buf[10];
	struct iovec iov[3];
	static int fd[2] = {-1, -1};

	if (fd[0] < 0) {		/* fork/exec our open server first time */
		if (s_pipe(fd) < 0) {
			perror("s_pipe error\n");
			exit(1);
		}
		if ((pid = fork()) < 0) {
			perror("fork error\n");
			exit(1);
		} else if (pid == 0) {		/* child */
			close(fd[0]);
			if (fd[1] != STDIN_FILENO) {
				if (dup2(fd[1], STDIN_FILENO) != STDIN_FILENO) {
					perror("dup2 error to stdin\n");
					exit(1);
				}
			}
			if (fd[1] != STDOUT_FILENO) {
				if (dup2(fd[2], STDOUT_FILENO) != STDOUT_FILENO) {
					perror("dup2 error to stdout\n");
					exit(1);
				}
			}
			if (execl("./opend", "opend", NULL) < 0) {
				perror("execl error\n");
				exit(1);
			}
		}
		close(fd[1]);			/* parenet */
	}

	sprintf(buf, " %d", oflag);		/* oflag to ascii */
	iov[0].iov_base = CL_OPEN " ";
	iov[0].iov_len = strlen(CL_OPEN) + 1;
	iov[1].iov_base = name;
	iov[1].iov_len = strlen(name);
	iov[2].iov_base = buf;
	iov[2].iov_len = strlen(buf) + 1;	/* +1 for null at end of buf */
	len = iov[0].iov_len + iov[1].iov_len + iov[2].iov_len;
	if (writev(fd[0], &iov[0], 3) != len) {
		perror("writev error\n");
		exit(1);
	}

					/* read descriptor, returned errors handled by write() */
	return(recv_fd(fd[0], write));
}
