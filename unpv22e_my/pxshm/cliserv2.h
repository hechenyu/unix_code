#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#define	MESGSIZE	256		/* max #bytes per message, incl. null at end */
#define	NMESG	16		/* max #messages */

struct shmstruct {		/* struct stored in shared memory */
	sem_t mutex;			/* three Posix memory-based semaphores */
	sem_t nempty;
	sem_t nstored;
	int nput;			/* index into msgoff[] for next put */
	long noverflow;		/* #overflows by senders */
	sem_t noverflowmutex;	/* mutex for noverflow counter */
	long msgoff[NMESG];	/* offset in shared memory of each message */
	char msgdata[NMESG * MESGSIZE];	/* the actual messages */
};
