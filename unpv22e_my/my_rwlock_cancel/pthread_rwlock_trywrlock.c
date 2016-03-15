/* include trywrlock */
#include <pthread.h>
#include <errno.h>
#include "pthread_rwlock.h"

int
mypthread_rwlock_trywrlock(mypthread_rwlock_t *rw)
{
	int		result;

	if (rw->rw_magic != RW_MAGIC)
		return(EINVAL);

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return(result);

	if (rw->rw_refcount != 0)
		result = EBUSY;			/* held by either writer or reader(s) */
	else
		rw->rw_refcount = -1;	/* available, indicate a writer has it */

	pthread_mutex_unlock(&rw->rw_mutex);
	return(result);
}
/* end trywrlock */

