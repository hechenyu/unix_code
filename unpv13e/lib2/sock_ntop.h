#ifndef	__unp_sock_cmp_addr_h
#define	__unp_sock_cmp_addr_h

#include <netinet/in.h>
#include <string.h>
#include <sys/un.h>

/* Define to 1 if the system supports IPv6 */
#define IPV6    1

char *sock_ntop(const struct sockaddr *sa, socklen_t salen);
char *Sock_ntop(const struct sockaddr *sa, socklen_t salen);

#endif

