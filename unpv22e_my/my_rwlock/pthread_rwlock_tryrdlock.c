/* include tryrdlock */
#include <pthread.h>
#include <errno.h>
#include "pthread_rwlock.h"

int
mypthread_rwlock_tryrdlock(mypthread_rwlock_t *rw)
{
	int		result;

	if (rw->rw_magic != RW_MAGIC)
		return(EINVAL);

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return(result);

	if (rw->rw_refcount < 0 || rw->rw_nwaitwriters > 0)
		result = EBUSY;			/* held by a writer or waiting writers */
	else
		rw->rw_refcount++;		/* increment count of reader locks */

	pthread_mutex_unlock(&rw->rw_mutex);
	return(result);
}
/* end tryrdlock */

