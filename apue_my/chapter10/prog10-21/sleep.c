/* 程序10-21 sleep的可靠实现 */
#include <signal.h>
#include <stddef.h>

static void sig_alrm(void)
{
	return;		/* nothing to do, just returning wakes up sigsuspend() */
}

unsigned int sleep(unsigned int nsecs)
{
	struct sigaction newact, oldact;
	sigset_t newmask, oldmask, suspmask;
	unsigned int unslep;

	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flag = 0;
	sigaction(SIGALRM, &newact, &oldact);
						/* set our handler, save previous information */

	sigemptyset(&newmask);
	sigaddset(&newmask, SIGALRM);
					/* block SIGALRM and save current signal mask */
	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

	alarm(nsecs);

	suspmask = oldmask;
	sigdelset(&suspmask, SIGALRM);	/* make sure SIGALRM isn't blocked */

	sigsuspend(&suspmask);		/* wait for any signal to caught */

	/* some signal has been caught, SIGALRM is now blocked */

	unslept = alarm(0);
	sigaction(SIGALRM, &oldact, NULL);	/* reset previous action */

						/* reset signal mask, which unblocks SIGALRM */
	sigprocmask(SIG_SETMASK, &oldmask, NULL);

	return(unslept);
}
