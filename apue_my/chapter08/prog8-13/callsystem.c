/* 程序8-13 调用system函数 */
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int my_system(const char *);
void pr_exit(int status);

int main(void)
{
	int status;

	if ((status = my_system("date")) < 0) {
		perror("my_system(\"date\") error");
/*		exit(1); */
	}
	pr_exit(status);

	if ((status = my_system("nosuchcommand")) < 0) {
		perror("my_system(\"nosuchcommand\") error");
/*		exit(1); */
	}
	pr_exit(status);

	if ((status = my_system("who; exit 44")) < 0) {
		perror("system(\"who; exit 44\") error");
/*		exit(1); */
	}
	pr_exit(status);

	if ((status = my_system("/bin/ed")) < 0) {
		perror("system(\"/bin/ed\") error");
/*		exit(1); */
	}
	pr_exit(status);

	exit(0);
}
