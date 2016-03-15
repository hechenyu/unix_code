#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

char	**my_addrs(int *);

int
main(int argc, char **argv)
{
	int		addrtype;
	char	**pptr, buf[INET6_ADDRSTRLEN];
	const char 	*addrstr;

	if ( (pptr = my_addrs(&addrtype)) == NULL) {
		perror("my_addrs error");
		exit(1);
	}

	for ( ; *pptr != NULL; pptr++) {
		addrstr = inet_ntop(addrtype, *pptr, buf, sizeof(buf));
		if (addrstr == NULL) {
			perror("inet_ntop error");
			exit(1);
		}
		printf("\taddress: %s\n", addrstr);
			   
	}

	exit(0);
}
