/* 程序15-14 opend.h头文件 */
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int recv_fd(int, ssize_t (*func)(int, const void *, size_t));
int send_fd(int, int);
int send_err(int, int, const char *);

#define	CL_OPEN	"open"			/* client's request for server */

			/* declare global variables */
extern char errmsg[];		/* error message string to return to client */
extern int oflag;			/* open() flag: O_xxx ... */
extern char *pathname;		/* of file to open() for client */

			/* function prototypes */
int cli_args(int, char **);
void request(char *, int, int);
