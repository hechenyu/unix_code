#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

ssize_t
read_fd(int fd, void *ptr, size_t nbytes, int *recvfd);

int
my_open(const char *pathname, int mode)
{
	int			fd, sockfd[2], status;
	pid_t		childpid;
	char		c, argsockfd[10], argmode[10];

	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd) < 0) {
		perror("socketpair error");
		exit(1);
	}

	if ((childpid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {		/* child process */
		if (close(sockfd[0]) < 0) {
			perror("close error");
			exit(1);
		}
		snprintf(argsockfd, sizeof(argsockfd), "%d", sockfd[1]);
		snprintf(argmode, sizeof(argmode), "%d", mode);
		execl("./openfile", "openfile", argsockfd, pathname, argmode,
			  (char *) NULL);
		perror("execl error");
		exit(1);
	}

	/* parent process - wait for the child to terminate */
	if (close(sockfd[1]) < 0) {			/* close the end we don't use */
		perror("close error");
		exit(1);
	}

	if (waitpid(childpid, &status, 0) == -1) {
		perror("waitpid error");
		exit(1);
	}
	if (WIFEXITED(status) == 0) {
		fprintf(stderr, "child did not terminate\n");
		exit(1);
	}
	if ( (status = WEXITSTATUS(status)) == 0) {
		if (read_fd(sockfd[0], &c, 1, &fd) < 0) {
			perror("read_fd error");
			exit(1);
		}
	} else {
		errno = status;		/* set errno value from child's status */
		fd = -1;
	}

	if (close(sockfd[0]) < 0) {
		perror("close error");
		exit(1);
	}
	return(fd);
}
