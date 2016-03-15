#ifndef MSGBUF_H
#define MSGBUF_H
struct msgbuf {
	long mtype;    /* type of received/sent message */
	char mtext[1];      /* text of the message */
};
#endif
