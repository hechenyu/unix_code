/* include mesg_send */
#include "mesg.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t mesg_send(int fd, struct mymesg *mptr)
{
	ssize_t	n = MESGHDRSIZE + mptr->mesg_len;
	if (write(fd, mptr, n) != n) {
		perror("mesg_send error");
		exit(1);
	}
	return n;
}
/* end mesg_send */

