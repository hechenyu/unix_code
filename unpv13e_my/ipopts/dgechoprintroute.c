#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#define MAXLINE     4096    /* max text line length */

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef HAVE_BZERO
#define bzero(ptr,n)        memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

void
inet6_srcrt_print(void *ptr);

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int				n;
	char			mesg[MAXLINE];
	int				on;
	char			control[MAXLINE];
	struct msghdr	msg;
	struct cmsghdr	*cmsg;
	struct iovec	iov[1];

	on = 1;
	if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVRTHDR, &on, sizeof(on)) < 0) {
		perror("setsockopt error");
		exit(1);
	}

	bzero(&msg, sizeof(msg));
	iov[0].iov_base = mesg;
	msg.msg_name = pcliaddr;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control;
	for ( ; ; ) {
		msg.msg_namelen = clilen;
		msg.msg_controllen = sizeof(control);
		iov[0].iov_len = MAXLINE;
		if ((n = recvmsg(sockfd, &msg, 0)) < 0) {
			perror("recvmsg error");
			exit(1);
		}

		for (cmsg = CMSG_FIRSTHDR(&msg); cmsg != NULL;
			 cmsg = CMSG_NXTHDR(&msg, cmsg)) {
			if (cmsg->cmsg_level == IPPROTO_IPV6 &&
				cmsg->cmsg_type == IPV6_RTHDR) {
				inet6_srcrt_print(CMSG_DATA(cmsg));
				inet6_rth_reverse(CMSG_DATA(cmsg), CMSG_DATA(cmsg));
			}
		}

		iov[0].iov_len = n;
		int nbytes = 0;	/* must first figure out what return value should be */
		int i;
		for (i = 0; i < msg.msg_iovlen; i++)
			nbytes += msg.msg_iov[i].iov_len;
		if (sendmsg(sockfd, &msg, 0) != nbytes) {
			perror("sendmsg error");
			exit(1);
		}
	}
}
