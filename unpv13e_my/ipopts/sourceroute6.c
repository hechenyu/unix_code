#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>

void
inet6_srcrt_print(void *ptr)
{
	int			i, segments;
	char			str[INET6_ADDRSTRLEN];

	if ((segments = inet6_rth_segments(ptr)) < 0) {
		fprintf(stderr, "inet6_rth_segments error\n");
		exit(1);
	}
	printf("received source route: ");
	for (i = 0; i < segments; i++)
		printf("%s ", inet_ntop(AF_INET6, inet6_rth_getaddr(ptr, i),
								str, sizeof(str)));
	printf("\n");
}
