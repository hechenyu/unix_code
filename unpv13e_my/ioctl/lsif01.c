#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>	
#include <net/if.h>

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char **argv)
{
	int					sockfd, len;
	char				*ptr, buf[2048], addrstr[INET_ADDRSTRLEN];
	struct ifconf		ifc;
	struct ifreq		*ifr;
	struct sockaddr_in	*sinptr;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("sockfd error");
		exit(1);
	}

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_req = (struct ifreq *) buf;
	if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) {
		perror("ioctl error");
		exit(1);
	}

	for (ptr = buf; ptr < buf + ifc.ifc_len; ) {
		ifr = (struct ifreq *) ptr;
		len = sizeof(struct sockaddr);
#ifdef	HAVE_SOCKADDR_SA_LEN
		if (ifr->ifr_addr.sa_len > len)
			len = ifr->ifr_addr.sa_len;		/* length > 16 */
#endif
		ptr += sizeof(ifr->ifr_name) + len;	/* for next one in buffer */

		switch (ifr->ifr_addr.sa_family) {
		case AF_INET:
			sinptr = (struct sockaddr_in *) &ifr->ifr_addr;
			printf("%s\t%s\n", ifr->ifr_name,
				   inet_ntop(AF_INET, &sinptr->sin_addr, addrstr, sizeof(addrstr)));
			break;

		default:
			printf("%s\n", ifr->ifr_name);
			break;
		}
	}
	exit(0);
}
