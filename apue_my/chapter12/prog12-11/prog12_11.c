/* 程序12-11 用detmsg将标准输入复制到标准输出 */
#include <stropts.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	BUFFSIZE	8192

int main(void)
{
	int n, flag;
	char ctlbuf[BUFFSIZE], datbuf[BUFFSIZE];
	struct strbuf ctl, dat;

	ctl.buf = ctlbuf;
	ctl.maxlen = BUFFSIZE;
	dat.buf = datbuf;
	dat.maxlen = BUFFSIZE;
	for ( ; ; ) {
		flag = 0;			/* return any message */
		if ((n = getmsg(STDIN_FILENO, &ctl, &dat, &flag)) < 0) {
			perror("getmsg error");
			exit(1);
		}
		fprintf(stderr, "flag = %d, ctl.len = %d, dat.len = %d\n",
				flag, ctl.len, dat.len);

		if (dat.len == 0)
			exit(0);
		else if (dat.len > 0)
			if (write(STDOUT_FILENO, dat.buf, dat.len) != dat.len) {
				perror("write error");
				exit(1);
			}
	}
}
