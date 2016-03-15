/* include mesg_recv */
#include <sys/msg.h>
#include <errno.h>
#include	"mesg.h"

ssize_t
mesg_recv(int id, struct mymesg *mptr)
{
	ssize_t	n;

	do {
		n = msgrcv(id, &(mptr->mesg_type), MAXMESGDATA, mptr->mesg_type, 0);
	} while (n == -1 && errno == EINTR);
	mptr->mesg_len = n;		/* return #bytes of data */

	return(n);				/* -1 on error, 0 at EOF, else >0 */
}
/* end mesg_recv */
