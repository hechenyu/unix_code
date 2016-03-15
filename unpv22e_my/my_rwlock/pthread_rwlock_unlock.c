/* include unlock */
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "pthread_rwlock.h"

int
mypthread_rwlock_unlock(mypthread_rwlock_t *rw)
{
	int		result;

	if (rw->rw_magic != RW_MAGIC)
		return(EINVAL);

	if ((result = pthread_mutex_lock(&rw->rw_mutex)) != 0)
		return(result);

	if (rw->rw_refcount > 0)
		rw->rw_refcount--;			/* releasing a reader */
	else if (rw->rw_refcount == -1)
		rw->rw_refcount = 0;		/* releasing a reader */
	else {
		fprintf(stderr, "rw_refcount = %d\n", rw->rw_refcount);
		abort();
		exit(1);
	}

		/* 4give preference to waiting writers over waiting readers */
	if (rw->rw_nwaitwriters > 0) {
		if (rw->rw_refcount == 0)
			result = pthread_cond_signal(&rw->rw_condwriters);
	} else if (rw->rw_nwaitreaders > 0)
		result = pthread_cond_broadcast(&rw->rw_condreaders);

	pthread_mutex_unlock(&rw->rw_mutex);
	return(result);
}
/* end unlock */

