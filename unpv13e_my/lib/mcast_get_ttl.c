#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <errno.h>

int
sockfd_to_family(int sockfd);

int
mcast_get_ttl(int sockfd)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		u_char		ttl;
		socklen_t	len;

		len = sizeof(ttl);
		if (getsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL,
					   &ttl, &len) < 0)
			return(-1);
		return(ttl);
	}

#ifdef	AF_INET6
	case AF_INET6: {
		int			hop;
		socklen_t	len;

		len = sizeof(hop);
		if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
					   &hop, &len) < 0)
			return(-1);
		return(hop);
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

