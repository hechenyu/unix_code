/* 程序10-2 在信号处理程序中调用不可再入函数 */
#include <pwd.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

static void my_alarm(int);

int main(void)
{
	struct passwd *ptr;

	signal(SIGALRM, my_alarm);
	alarm(1);

	for ( ; ; ) {
		if ((ptr = getpwnam("post2")) == NULL) {
			perror("getpwnam error");
			exit(1);
		}
		if (strcmp(ptr->pw_name, "post2") != 0)
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
	}
}

static void my_alarm(int signo)
{
	struct passwd *rootptr;

	printf("in signal handler\n");
	if ((rootptr = getpwnam("root")) == NULL) {
		perror("getpwnam(root) error");
		exit(1);
	}
	alarm(1);
	return;
}
