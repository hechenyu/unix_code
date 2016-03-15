/* include mesg_send */
#include <sys/msg.h>
#include	"mesg.h"

ssize_t
mesg_send(int id, struct mymesg *mptr)
{
	return(msgsnd(id, &(mptr->mesg_type), mptr->mesg_len, 0));
}
/* end mesg_send */
