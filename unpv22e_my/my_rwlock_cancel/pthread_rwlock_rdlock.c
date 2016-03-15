/* include rdlock */
#include <pthread.h>
#include <errno.h>
#include "pthread_rwlock.h"

/* include rwlock_cancelrdwait */
static void
rwlock_cancelrdwait(void *arg)
{
	mypthread_rwlock_t	*rw;

	rw = arg;
	rw->rw_nwaitreaders--;
	pthread_mutex_unlock(&rw->rw_mutex);
}
/* end rwlock_cancelrdwait */

int
mypthread_rwlock_rdlock(mypthread_rwlock_t *rw)
{
	int		result;

	if (rw->rw_magic != RW_MAGIC)
		return(EINVAL);

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return(result);

		/* 4give preference to waiting writers */
	while (rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0) {
		rw->rw_nwaitreaders++;
		pthread_cleanup_push(rwlock_cancelrdwait, (void *) rw);
		result = pthread_cond_wait(&rw->rw_condreaders, &rw->rw_mutex);
		pthread_cleanup_pop(0);
		rw->rw_nwaitreaders--;
		if (result != 0)
			break;
	}
	if (result == 0)
		rw->rw_refcount++;		/* another reader has a read lock */

	pthread_mutex_unlock(&rw->rw_mutex);
	return (result);
}
/* end rdlock */

