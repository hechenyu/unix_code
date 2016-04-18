#ifndef __unp_signal_h
#define __unp_signal_h

#include "error.h"
#include <signal.h>

typedef	void	Sigfunc(int);	/* for signal handlers */

Sigfunc *Signal(int, Sigfunc *);

#endif
