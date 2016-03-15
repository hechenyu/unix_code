/* include dgsendrecv1 */
#include <sys/socket.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#include "rtt_info.h"

#define	RTT_DEBUG

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

static struct rtt_info   rttinfo;
static int	rttinit = 0;
static struct msghdr	msgsend, msgrecv;	/* assumed init to 0 */
static struct hdr {
  uint32_t	seq;	/* sequence # */
  uint32_t	ts;		/* timestamp when sent */
} sendhdr, recvhdr;

static void	sig_alrm(int signo);
static sigjmp_buf	jmpbuf;

ssize_t
dg_send_recv(int fd, const void *outbuff, size_t outbytes,
			 void *inbuff, size_t inbytes,
			 const SA *destaddr, socklen_t destlen)
{
	ssize_t			n;
	struct iovec	iovsend[2], iovrecv[2];

	if (rttinit == 0) {
		rtt_init(&rttinfo);		/* first time we're called */
		rttinit = 1;
		rtt_d_flag = 1;
	}

	sendhdr.seq++;
	msgsend.msg_name = destaddr;
	msgsend.msg_namelen = destlen;
	msgsend.msg_iov = iovsend;
	msgsend.msg_iovlen = 2;
	iovsend[0].iov_base = &sendhdr;
	iovsend[0].iov_len = sizeof(struct hdr);
	iovsend[1].iov_base = outbuff;
	iovsend[1].iov_len = outbytes;

	msgrecv.msg_name = NULL;
	msgrecv.msg_namelen = 0;
	msgrecv.msg_iov = iovrecv;
	msgrecv.msg_iovlen = 2;
	iovrecv[0].iov_base = &recvhdr;
	iovrecv[0].iov_len = sizeof(struct hdr);
	iovrecv[1].iov_base = inbuff;
	iovrecv[1].iov_len = inbytes;
/* end dgsendrecv1 */

/* include dgsendrecv2 */
	if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
		perror("signal error");
		exit(1);
	}
	rtt_newpack(&rttinfo);		/* initialize for this packet */

sendagain:
#ifdef	RTT_DEBUG
	fprintf(stderr, "send %4d: ", sendhdr.seq);
#endif
	sendhdr.ts = rtt_ts(&rttinfo);
	int nbytes = 0;	/* must first figure out what return value should be */
	int i;
	for (i = 0; i < msgsend.msg_iovlen; i++)
		nbytes += msgsend.msg_iov[i].iov_len;
	if (sendmsg(fd, &msgsend, 0) != nbytes) {
		perror("sendmsg error");
		exit(1);
	}

	alarm(rtt_start(&rttinfo));	/* calc timeout value & start timer */
#ifdef	RTT_DEBUG
	rtt_debug(&rttinfo);
#endif

	if (sigsetjmp(jmpbuf, 1) != 0) {
		if (rtt_timeout(&rttinfo) < 0) {
			fprintf(stderr, "dg_send_recv: no response from server, giving up\n");
			rttinit = 0;	/* reinit in case we're called again */
			errno = ETIMEDOUT;
			return(-1);
		}
#ifdef	RTT_DEBUG
		fprintf(stderr, "dg_send_recv: timeout, retransmitting\n");
#endif
		goto sendagain;
	}

	do {
		if ((n = recvmsg(fd, &msgrecv, 0)) < 0) {
			perror("recvmsg error");
			exit(1);
		}
#ifdef	RTT_DEBUG
		fprintf(stderr, "recv %4d\n", recvhdr.seq);
#endif
	} while (n < sizeof(struct hdr) || recvhdr.seq != sendhdr.seq);

	alarm(0);			/* stop SIGALRM timer */
		/* 4calculate & store new RTT estimator values */
	rtt_stop(&rttinfo, rtt_ts(&rttinfo) - recvhdr.ts);

	return(n - sizeof(struct hdr));	/* return size of received datagram */
}

static void
sig_alrm(int signo)
{
	siglongjmp(jmpbuf, 1);
}
/* end dgsendrecv2 */
