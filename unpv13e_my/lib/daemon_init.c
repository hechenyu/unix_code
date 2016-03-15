#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>

#define	MAXFD	64

int
daemon_init(const char *pname, int facility)
{
	int		i;
	pid_t	pid;

	if ( (pid = fork()) < 0) {
		perror("fork error");
		return (-1);
	} else if (pid) {
		_exit(0);			/* parent terminates */
	}

	/* child 1 continues... */

	if (setsid() < 0)			/* become session leader */
		return (-1);

	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		perror("signal error");
		return (-1);
	}
	if ( (pid = fork()) < 0) {
		perror("fork error");
		return (-1);
	 }else if (pid) {
		_exit(0);			/* child 1 terminates */
	}

	/* child 2 continues... */

	chdir("/");				/* change working directory */

	/* close off file descriptors */
	for (i = 0; i < MAXFD; i++)
		close(i);

	/* redirect stdin, stdout, and stderr to /dev/null */
	open("/dev/null", O_RDONLY);
	open("/dev/null", O_RDWR);
	open("/dev/null", O_RDWR);

	openlog(pname, LOG_PID, facility);

	return (0);				/* success */
}
