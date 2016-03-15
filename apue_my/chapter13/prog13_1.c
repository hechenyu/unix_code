/* 程序13-1 初始化一个精灵进程 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int deamon_init(void);

int main(void)
{
	deamon_init();
	sleep(50);
	return 0;
}

int deamon_init(void)
{
	pid_t pid;

	if ((pid = fork()) < 0)
		return(-1);
	else if (pid != 0)
		exit(0);	/* parent goes bye-bye */

	/* child continues */
	setsid();		/* become session leader */

	chdir("/");		/* change working directory */

	umask(0);		/* clear our file mode creation mask */

	return(0);
}
