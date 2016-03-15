/* include readable_v61 */
#include <arpa/inet.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <sys/un.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef	AF_INET6
#include	<netinet/ip6.h>
#include	<netinet/icmp6.h>
#endif

#include "icmpd.h"

#define MAXLINE     4096    /* max text line length */

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

char *
sock_ntop_host(const struct sockaddr *sa, socklen_t salen);

readable_v6(void)
{
#ifdef	AF_INET6
	int					i, hlen2, icmp6len, sport;
	char				buf[MAXLINE];
	char				srcstr[INET6_ADDRSTRLEN], dststr[INET6_ADDRSTRLEN];
	ssize_t				n;
	socklen_t			len;
	struct ip6_hdr		*ip6, *hip6;
	struct icmp6_hdr	*icmp6;
	struct udphdr		*udp;
	struct sockaddr_in6	from, dest;
	struct icmpd_err	icmpd_err;

	len = sizeof(from);
	if ((n = recvfrom(fd6, buf, MAXLINE, 0, (SA *) &from, &len)) < 0) {
		perror("recvfrom error");
		exit(1);
	}

	printf("%d bytes ICMPv6 from %s:",
		   n, sock_ntop_host((SA *) &from, len));

	icmp6 = (struct icmp6_hdr *) buf;		/* start of ICMPv6 header */
	if ( (icmp6len = n) < 8) {
		fprintf(stderr, "icmp6len (%d) < 8\n", icmp6len);
		exit(1);
	}

	printf(" type = %d, code = %d\n", icmp6->icmp6_type, icmp6->icmp6_code);
/* end readable_v61 */

/* include readable_v62 */
	if (icmp6->icmp6_type == ICMP6_DST_UNREACH ||
		icmp6->icmp6_type == ICMP6_PACKET_TOO_BIG ||
		icmp6->icmp6_type == ICMP6_TIME_EXCEEDED) {
		if (icmp6len < 8 + 8) {
			fprintf(stderr, "icmp6len (%d) < 8 + 8\n", icmp6len);
			exit(1);
		}

		hip6 = (struct ip6_hdr *) (buf + 8);
		hlen2 = sizeof(struct ip6_hdr);
		printf("\tsrcip = %s, dstip = %s, next hdr = %d\n",
			   inet_ntop(AF_INET6, &hip6->ip6_src, srcstr, sizeof(srcstr)),
			   inet_ntop(AF_INET6, &hip6->ip6_dst, dststr, sizeof(dststr)),
			   hip6->ip6_nxt);
 		if (hip6->ip6_nxt == IPPROTO_UDP) {
			udp = (struct udphdr *) (buf + 8 + hlen2);
			sport = udp->uh_sport;

				/* 4find client's Unix domain socket, send headers */
			for (i = 0; i <= maxi; i++) {
				if (client[i].connfd >= 0 &&
					client[i].family == AF_INET6 &&
					client[i].lport == sport) {

					bzero(&dest, sizeof(dest));
					dest.sin6_family = AF_INET6;
#ifdef	HAVE_SOCKADDR_SA_LEN
					dest.sin6_len = sizeof(dest);
#endif
					memcpy(&dest.sin6_addr, &hip6->ip6_dst,
						   sizeof(struct in6_addr));
					dest.sin6_port = udp->uh_dport;

					icmpd_err.icmpd_type = icmp6->icmp6_type;
					icmpd_err.icmpd_code = icmp6->icmp6_code;
					icmpd_err.icmpd_len = sizeof(struct sockaddr_in6);
					memcpy(&icmpd_err.icmpd_dest, &dest, sizeof(dest));

						/* 4convert type & code to reasonable errno value */
					icmpd_err.icmpd_errno = EHOSTUNREACH;	/* default */
					if (icmp6->icmp6_type == ICMP6_DST_UNREACH &&
						icmp6->icmp6_code == ICMP6_DST_UNREACH_NOPORT)
						icmpd_err.icmpd_errno = ECONNREFUSED;
					if (icmp6->icmp6_type == ICMP6_PACKET_TOO_BIG)
							icmpd_err.icmpd_errno = EMSGSIZE;
					if (write(client[i].connfd, &icmpd_err, sizeof(icmpd_err)) != sizeof(icmpd_err)) {
						perror("write error");
						exit(1);
					}
				}
			}
		}
	}
	return(--nready);
#endif
}
/* end readable_v62 */
