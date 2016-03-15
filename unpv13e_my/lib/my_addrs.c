/* include my_addrs */
#include <sys/utsname.h>
#include <netdb.h>

#include <stddef.h>
#include <stdio.h>

char **
my_addrs(int *addrtype)
{
	struct hostent	*hptr;
	struct utsname	myname;

	if (uname(&myname) < 0)
		return(NULL);

	printf("myname: %s\n", myname.nodename);
	if ( (hptr = gethostbyname(myname.nodename)) == NULL)
		return(NULL);

	*addrtype = hptr->h_addrtype;
	return(hptr->h_addr_list);
}
/* end my_addrs */
