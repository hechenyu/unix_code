#ifndef UNP_IN_PKTINFO_H 
#define UNP_IN_PKTINFO_H 

#include <netinet/in.h>

/* The structure returned by recvfrom_flags() */
struct unp_in_pktinfo {
  struct in_addr	ipi_addr;	/* dst IPv4 address */
  int				ipi_ifindex;/* received interface index */
};

#endif
