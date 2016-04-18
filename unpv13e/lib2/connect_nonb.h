#ifndef __unp_connect_nonb_h
#define __unp_connect_nonb_h

#include "error.h"
#include "wrapunix.h"
#include "wrapsock.h"

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */

#ifndef SA
#define	SA	struct sockaddr
#endif

int		 connect_nonb(int, const SA *, socklen_t, int);

#endif
