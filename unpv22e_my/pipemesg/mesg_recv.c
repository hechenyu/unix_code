/* include mesg_recv */
#include "mesg.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

ssize_t mesg_recv(int fd, struct mymesg *mptr)
{
	size_t	len;
	ssize_t	n;

	/* 4read message header first, to get len of data that follows */
	if ((n = read(fd, mptr, MESGHDRSIZE)) < 0) {
		perror("read error");
		exit(1);
	} else if (n == 0) {
		return(0);		/* end of file */
	} else if (n != MESGHDRSIZE) {
		fprintf(stderr, "message header: expected %d, got %d\n", MESGHDRSIZE, n);
		exit(1);
	}

	if ((len = mptr->mesg_len) > 0) {
		if ((n = read(fd, mptr->mesg_data, len)) < 0) {
			perror("read error");
			exit(1);
		}
		if (n != len) {
			fprintf(stderr, "message data: expected %d, got %d\n", len, n);
			exit(1);
		}
	}
	return(len);
}
/* end mesg_recv */

