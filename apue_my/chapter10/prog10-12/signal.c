/* 程序10-12 用sigaction所实现的signal函数 */
/* Reliable version fo signal(), using POSIX sigaction(). */
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

typedef void Sigfunc(int);		/* for signal handlers */

Sigfunc *signal(int signo, Sigfunc *func)
{
	struct sigaction act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;		/* SunOS */
#endif
	} else {
#ifdef	SA_RESTART
		act.sa_flags |= SA_RESTART;			/* SVR4, 4.3+BSD */
#endif
	}
	if (sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}
		
