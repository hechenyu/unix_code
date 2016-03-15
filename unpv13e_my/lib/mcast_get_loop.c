#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <errno.h>

int
sockfd_to_family(int sockfd);

int
mcast_get_loop(int sockfd)
{
	switch (sockfd_to_family(sockfd)) {
	case AF_INET: {
		u_char		flag;
		socklen_t	len;

		len = sizeof(flag);
		if (getsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP,
					   &flag, &len) < 0)
			return(-1);
		return(flag);
	}

#ifdef	AF_INET6
	case AF_INET6: {
		u_int		flag;
		socklen_t	len;

		len = sizeof(flag);
		if (getsockopt(sockfd, IPPROTO_IPV6, IPV6_MULTICAST_LOOP,
					   &flag, &len) < 0)
			return(-1);
		return(flag);
	}
#endif

	default:
		errno = EAFNOSUPPORT;
		return(-1);
	}
}

