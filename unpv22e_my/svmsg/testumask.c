#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define	SVMSG_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV message queues */

int
main(int argc, char **argv)
{
	if (msgget(IPC_PRIVATE, 0666 | IPC_CREAT | IPC_EXCL) == -1) {
		perror("msgget error");
		exit(1);
	}
	unlink("/tmp/fifo.1");
	if (mkfifo("/tmp/fifo.1", 0666) == -1) {
		perror("mkfifo error");
		exit(1);
	}

	exit(0);
}
