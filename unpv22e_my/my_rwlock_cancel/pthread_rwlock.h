/* include rwlockh */
#ifndef	__mypthread_rwlock_h
#define	__mypthread_rwlock_h

#include <pthread.h>

typedef struct {
  pthread_mutex_t	rw_mutex;		/* basic lock on this struct */
  pthread_cond_t	rw_condreaders;	/* for reader threads waiting */
  pthread_cond_t	rw_condwriters;	/* for writer threads waiting */
  int				rw_magic;		/* for error checking */
  int				rw_nwaitreaders;/* the number waiting */
  int				rw_nwaitwriters;/* the number waiting */
  int				rw_refcount;
	/* 4-1 if writer has the lock, else # readers holding the lock */
} mypthread_rwlock_t;

#define	RW_MAGIC	0x19283746

	/* 4following must have same order as elements in struct above */
#define	MYPTHREAD_RWLOCK_INITIALIZER	{ PTHREAD_MUTEX_INITIALIZER, \
			PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, \
			RW_MAGIC, 0, 0, 0 }

typedef	int		mypthread_rwlockattr_t;	/* dummy; not supported */

	/* 4function prototypes */
int		mypthread_rwlock_destroy(mypthread_rwlock_t *);
int		mypthread_rwlock_init(mypthread_rwlock_t *, mypthread_rwlockattr_t *);
int		mypthread_rwlock_rdlock(mypthread_rwlock_t *);
int		mypthread_rwlock_tryrdlock(mypthread_rwlock_t *);
int		mypthread_rwlock_trywrlock(mypthread_rwlock_t *);
int		mypthread_rwlock_unlock(mypthread_rwlock_t *);
int		mypthread_rwlock_wrlock(mypthread_rwlock_t *);
/* $$.bp$$ */
	/* 4and our wrapper functions */

#endif	/* __mypthread_rwlock_h */
/* end rwlockh */
