#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#define MSG_R	S_IRUSR 
#define	MAXMSG	(8192 + sizeof(long))

void *
my_shm(size_t nbytes);

int
main(int argc, char **argv)
{
	int		pipe1[2], pipe2[2], mqid;
	char	c;
	pid_t	childpid;
	fd_set	rset;
	ssize_t	n, nread;
	struct msgbuf	*buff;
	key_t	key;

	if (argc != 2) {
		fprintf(stderr, "usage: svmsgread <pathname>\n");
		exit(1);
	}

	if (pipe(pipe1) < 0) {	/* 2-way communication with child */
		perror("pipe error");
		exit(1);
	}
	if (pipe(pipe2) < 0) {
		perror("pipe error");
		exit(1);
	}

	if ((buff = my_shm(MAXMSG)) == MAP_FAILED) {	/* anonymous shared memory with child */
		perror("my_shm error");
		exit(1);
	}

	if ((childpid = fork()) < 0) {
		perror("fork error");
		exit(1);
	} else if (childpid == 0) {
		if (close(pipe1[1]) == -1) {			/* child */
			perror("close error");
			exit(1);
		}
		if (close(pipe2[0]) == -1) {
			perror("close error");
			exit(1);
		}

		if ((key = ftok(argv[1], 0)) == -1) {
			fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
				argv[1], 0, strerror(errno));
		}
		if ((mqid = msgget(key, MSG_R)) == -1) {
			perror("msgget error");
			exit(1);
		}
		for ( ; ; ) {
				/* 4block, waiting for message, then tell parent */
			if ((nread = msgrcv(mqid, buff, MAXMSG, 0, 0)) == -1) {
				perror("msgrcv error");
				exit(1);
			}
			if (write(pipe2[1], &nread, sizeof(ssize_t)) != sizeof(ssize_t)) {
				perror("write error");
				exit(1);
			}

				/* 4wait for parent to say shm is available */
			if ((n = read(pipe1[0], &c, 1)) == -1) {
				perror("read error");
				exit(1);
			} else if (n != 1) {
				fprintf(stderr, "child: read on pipe returned %d\n", n);
				exit(1);
			}
		}
		exit(0);
	}
/* $$.bp$$ */
		/* 4parent */
	if (close(pipe1[0]) == -1) {
		perror("close error");
		exit(1);
	}
	if (close(pipe2[1]) == -1) {
		perror("close error");
		exit(1);
	}

	FD_ZERO(&rset);
	FD_SET(pipe2[0], &rset);
	for ( ; ; ) {
		if ((n = select(pipe2[0] + 1, &rset, NULL, NULL, NULL)) != 1) {
			fprintf(stderr, "select returned %d: %s\n", n, strerror(errno));
			exit(1);
		}
		if (FD_ISSET(pipe2[0], &rset)) {
			if ((n = read(pipe2[0], &nread, sizeof(ssize_t))) == -1) {
				perror("read error");
				exit(1);
			}
/* *INDENT-OFF* */
			if (n != sizeof(ssize_t)) {
				fprintf(stderr, "parent: read on pipe returned %d\n", n);
				exit(1);
			}
/* *INDENT-ON* */
			printf("read %d bytes, type = %ld\n", nread, buff->mtype);
			if (write(pipe1[1], &c, 1) != 1) {
				perror("write error");
				exit(1);
			}

		} else {
			fprintf(stderr, "pipe2[0] not ready\n");
			exit(1);
		}
	}

	if (kill(childpid, SIGTERM) == -1) {
		perror("kill error");
		exit(1);
	}
	exit(0);
}
