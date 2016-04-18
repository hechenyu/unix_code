#ifndef __unp_connect_timeo_h
#define __unp_connect_timeo_h

#include "error.h"
#include "signal_.h"

#include <unistd.h>
#include <signal.h>

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */

#ifndef SA
#define	SA	struct sockaddr
#endif

void	 Connect_timeo(int, const SA *, socklen_t, int);

#endif
