#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>		/* for IFNAMSIZ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "unp_in_pktinfo.h"

/* Define bzero() as a macro if it's not in standard C library. */
#ifndef HAVE_BZERO
#define bzero(ptr,n)        memset(ptr, 0, n)
/* $$.If bzero$$ */
/* $$.If memset$$ */
#endif

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

#undef	MAXLINE
#define	MAXLINE	20		/* to see datagram truncation */

ssize_t
recvfrom_flags(int fd, void *ptr, size_t nbytes, int *flagsp,
			   SA *sa, socklen_t *salenptr, struct unp_in_pktinfo *pktp);

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

void
dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int						flags;
	const int				on = 1;
	socklen_t				len;
	ssize_t					n;
	char					mesg[MAXLINE], str[INET6_ADDRSTRLEN],
							ifname[IFNAMSIZ];
	struct in_addr			in_zero;
	struct unp_in_pktinfo	pktinfo;

#ifdef IP_PKTINFO	
	if (setsockopt(sockfd, IPPROTO_IP, IP_PKTINFO, &on, sizeof(on)) < 0) {
		perror("setsockopt of IP_PKTINFO");
		exit(1);
	}
#else /* IP_PKTINFO	 */

#ifdef	IP_RECVDSTADDR
	if (setsockopt(sockfd, IPPROTO_IP, IP_RECVDSTADDR, &on, sizeof(on)) < 0) {
		perror("setsockopt of IP_RECVDSTADDR");
		exit(1);
	}
#endif
#ifdef	IP_RECVIF
	if (setsockopt(sockfd, IPPROTO_IP, IP_RECVIF, &on, sizeof(on)) < 0) {
		perror("setsockopt of IP_RECVIF");
		exit(1);
	}
#endif

#endif	/* IP_PKTINFO */
	bzero(&in_zero, sizeof(struct in_addr));	/* all 0 IPv4 address */

	for ( ; ; ) {
		len = clilen;
		flags = 0;
		if ((n = recvfrom_flags(sockfd, mesg, MAXLINE, &flags,
						   pcliaddr, &len, &pktinfo)) < 0) {
			perror("recvfrom_flags error");
			exit(1);
		}
		printf("%d-byte datagram from %s", n, sock_ntop(pcliaddr, len));
		if (memcmp(&pktinfo.ipi_addr, &in_zero, sizeof(in_zero)) != 0)
			printf(", to %s", inet_ntop(AF_INET, &pktinfo.ipi_addr,
										str, sizeof(str)));
		if (pktinfo.ipi_ifindex > 0)
			printf(", recv i/f = %d", pktinfo.ipi_ifindex); 
#if 0
			printf(", recv i/f = %s",
				   If_indextoname(pktinfo.ipi_ifindex, ifname));
#endif
#ifdef	MSG_TRUNC
		if (flags & MSG_TRUNC)	printf(" (datagram truncated)");
#endif
#ifdef	MSG_CTRUNC
		if (flags & MSG_CTRUNC)	printf(" (control info truncated)");
#endif
#ifdef	MSG_BCAST
		if (flags & MSG_BCAST)	printf(" (broadcast)");
#endif
#ifdef	MSG_MCAST
		if (flags & MSG_MCAST)	printf(" (multicast)");
#endif
		printf("\n");

		if (sendto(sockfd, mesg, n, 0, pcliaddr, len) != n) {
			perror("sendto error");
			exit(1);
		}
	}
}
