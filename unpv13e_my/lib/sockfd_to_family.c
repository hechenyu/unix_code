/* include sockfd_to_family */
#include <sys/socket.h>

/* Following shortens all the typecasts of pointer arguments: */
#define SA  struct sockaddr

int
sockfd_to_family(int sockfd)
{
	struct sockaddr_storage ss;
	socklen_t	len;

	len = sizeof(ss);
	if (getsockname(sockfd, (SA *) &ss, &len) < 0)
		return(-1);
	return(ss.ss_family);
}
/* end sockfd_to_family */

