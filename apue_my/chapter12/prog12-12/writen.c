/* ³ÌÐò12-12 writenº¯Êý */
#include <unistd.h>
#include <stdio.h>

ssize_t writen(int fd, const void *vptr, size_t n)	/* Write "n" bytes to a descriptor. */
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;		/* can't to do pointer arithmetic on void* */
	nleft = n;
	while (nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) <= 0) 
			return(nwritten);		/* error */

		nleft -= nwritten;
		ptr += nwritten;
	}
	return(n);
}
