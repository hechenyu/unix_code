#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>	
#include <net/if.h>
#include <net/if_arp.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ifi_info.h"

struct ifi_info *
get_ifi_info(int family, int doaliases);

char *
sock_ntop(const struct sockaddr *sa, socklen_t salen);

int
main(int argc, char **argv)
{
	int					sockfd;
	struct ifi_info			*ifi;
	unsigned char		*ptr;
	struct arpreq		arpreq;
	struct sockaddr_in	*sin;

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}
	for (ifi = get_ifi_info(AF_INET, 0); ifi != NULL; ifi = ifi->ifi_next) {
		printf("%s: ", sock_ntop(ifi->ifi_addr, sizeof(struct sockaddr_in)));

		sin = (struct sockaddr_in *) &arpreq.arp_pa;
		memcpy(sin, ifi->ifi_addr, sizeof(struct sockaddr_in));

#if 0
		if (ioctl(sockfd, SIOCGARP, &arpreq) < 0) {
			perror("ioctl SIOCGARP");
			continue;
		}

		ptr = (unsigned char *) &arpreq.arp_ha.sa_data[0];
		printf("%x:%x:%x:%x:%x:%x\n", *ptr, *(ptr+1),
			   *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
#else
		ptr = (unsigned char *) ifi->ifi_haddr;
		printf("%x:%x:%x:%x:%x:%x\n", *ptr, *(ptr+1),
			   *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5));
#endif
	}
	exit(0);
}
