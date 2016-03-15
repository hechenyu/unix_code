/* include wrlock */
#include <pthread.h>
#include <errno.h>
#include "pthread_rwlock.h"

/* include rwlock_cancelwrwait */
static void
rwlock_cancelwrwait(void *arg)
{
	mypthread_rwlock_t	*rw;

	rw = arg;
	rw->rw_nwaitwriters--;
	pthread_mutex_unlock(&rw->rw_mutex);
}
/* end rwlock_cancelwrwait */

int
mypthread_rwlock_wrlock(mypthread_rwlock_t *rw)
{
	int		result;

	if (rw->rw_magic != RW_MAGIC)
		return(EINVAL);

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return(result);

	while (rw->rw_refcount != 0) {
		rw->rw_nwaitwriters++;
		pthread_cleanup_push(rwlock_cancelwrwait, (void *) rw);
		result = pthread_cond_wait(&rw->rw_condwriters, &rw->rw_mutex);
		pthread_cleanup_pop(0);
		rw->rw_nwaitwriters--;
		if (result != 0)
			break;
	}
	if (result == 0)
		rw->rw_refcount = -1;

	pthread_mutex_unlock(&rw->rw_mutex);
	return(result);
}
/* end wrlock */

