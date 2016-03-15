#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

char	**my_addrs(int *);

int
main(int argc, char **argv)
{
	int		addrtype;
	char	**pptr, buf[INET6_ADDRSTRLEN];

	if ( (pptr = my_addrs(&addrtype)) == NULL) {
		perror("my_addrs error");
		exit(1);
	}

	for ( ; *pptr != NULL; pptr++)
		printf("\taddress: %s\n",
			   inet_ntop(addrtype, *pptr, buf, sizeof(buf)));

	exit(0);
}
