#ifndef __unp_signal_unp_h
#define __unp_signal_unp_h

#include <signal.h>
#include "error.h"

typedef	void	Sigfunc(int);	/* for signal handlers */

Sigfunc *Signal(int, Sigfunc *);

#endif
