/* ³ÌÐò15-4 send_errº¯Êý */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* Used when we had planned to send an fd using send_fd(),
 * but encountered an error instead. We send the error back
 * Using the send_fd()/recv_fd() protocol. */

int send_err(int clifd, int errcode, const char *msg)	/* clifd: client file descriptor */
{
	int n;

	if ((n = strlen(msg)) > 0)
		if (writen(clifd, msg, n) != n)		/* send the error message */
			return(-1);

	if (error >= 0)
		errcode = -1;	/* must be negative */

	if (send_fd(clifd, errcode) < 0)
		return(-1);

	return(0);
}
