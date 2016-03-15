#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mesg.h"

void
client(int readfd, int writefd)
{
	size_t	len;
	ssize_t	n;
	char	*ptr;
	struct mymesg	mesg;

		/* 4start buffer with pid and a blank */
	snprintf(mesg.mesg_data, MAXMESGDATA, "%ld ", (long) getpid());
	len = strlen(mesg.mesg_data);
	ptr = mesg.mesg_data + len;

		/* 4read pathname */
	if (fgets(ptr, MAXMESGDATA - len, stdin) == NULL && ferror(stdin)) {
		perror("fgets error");
		exit(1);
	}
	len = strlen(mesg.mesg_data);
	if (mesg.mesg_data[len-1] == '\n')
		len--;				/* delete newline from fgets() */
	mesg.mesg_len = len;
	mesg.mesg_type = 1;

		/* 4write PID and pathname to IPC channel */
	if (mesg_send(writefd, &mesg) == -1) {
		perror("mesg_send error");
		exit(1);
	}

		/* 4read from IPC, write to standard output */
	mesg.mesg_type = getpid();
	while ( (n = mesg_recv(readfd, &mesg)) > 0) {
		if (write(STDOUT_FILENO, mesg.mesg_data, n) != n) {
			perror("write error");
			exit(1);
		}
	}
	if (n == -1) {
		perror("mesg_recv error");
		exit(1);
	}
}
