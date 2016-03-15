/* ³ÌÐò15-6 SVR4µÄrecv_fdº¯Êý */
#include <sys/types.h>
#include <stropts.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	MAXLINE	4096

/* Receive a file descriptor from another process (a server).
 * In addition, anv data received from the server is passed
 * to (*userfunc)(STDERR_FILENO, buf, nbytes). We have a
 * 2-byte protocol for receiving the fd from send_fd(). */

int recv_fd(int servfd, ssize_t (*userfunc)(int, const void *, size_t))
{									/* servfd: server file descriptor */
	int newfd, nread, flag, status;
	char *ptr, buf[MAXLINE];
	struct strrecvfd recvfd;

	status = -1;
	for ( ; ; ) {
		dat.buf = buf;
		dat.maxlen = MAXLINE;
		flag = 0;
		if (getmsg(servfd, NULL, &date, &flag) < 0) {
			perror("getmsg error\n");
			exit(1);
		}
		nread = dat.len;
		if (nread == 0) {
			perror("connection closed by server\n");
			return(-1);
		}
		/* See if this is the final data with null & status.
		   Null must be next to last byte of buffer, status
		   byte is last byte. Zero status means there must
		   be a file descriptor to receive. */
		for (ptr = buf; ptr < &buf[nread]; ) {
			if (*ptr++ == 0) {
				if (ptr != &buf[nread-1]) {
					perror("message format error");
					exit(1);
				}
				status = *ptr & 255;
				if (status == 0) {
					if (ioctl(servfd, I_RECVFD, &recvfd) < 0)
						return(-1);
					newfd = recvfd.fd;		/* new descriptor */
				} else
					newfd = -status;
				nread -= 2;
			}
		}
		if (nread > 0)
			if ((*userfunc)(STDERR_FILENO, buf, nread) != nread)
				return(-1);

		if (status >= 0)	/* final data has arrived */
			return(newfd);	/* descriptor, or -status */
	}
}
