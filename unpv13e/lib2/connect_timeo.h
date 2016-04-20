#ifndef __unp_connect_timeo_h
#define __unp_connect_timeo_h

#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include "error.h"
#include "signal_unp.h"

#ifndef SA
#define	SA	struct sockaddr
#endif

void	 Connect_timeo(int, const SA *, socklen_t, int);

#endif
