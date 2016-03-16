#ifndef CONNECT_NONB_H
#define CONNECT_NONB_H

//#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */

#ifndef SA
#define	SA	struct sockaddr
#endif

int		 connect_nonb(int, const SA *, socklen_t, int);

#endif
