#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>	
#include <net/if.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int
main(int argc, char **argv)
{
	int		i, sockfd, numif;
	char	buf[1024];

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket error");
		exit(1);
	}

	numif = 999;
	if (ioctl(sockfd, SIOCGIFNUM, &numif) < 0) {
		perror("ioctl error");
		exit(1);
	}
	printf("numif = %d\n", numif);

	i = ioctl(sockfd, SIOCGHIWAT, &buf);
	printf("i = %d, errno = %d\n", i, errno);
	exit(0);
}
