#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <errno.h>

int
sockfd_to_family(int sockfd);

int
mcast_set_ttl(int sockfd, int val)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		u_char		ttl;

		ttl = val;
		return(setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL,
						  &ttl, sizeof(ttl)));
	}

#ifdef	AF_INET6
	case AF_INET6: {
		int	hop;

		hop = val;
		return(setsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
						  &hop, sizeof(hop)));
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

