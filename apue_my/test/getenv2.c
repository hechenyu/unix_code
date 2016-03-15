/* DEBUG的新途径 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int flag;

int main()
{
	char *pstr;

	if ((pstr = getenv("DEBUGON")) == NULL)
		flag = 0;
	else
		flag = 1;

	if (flag == 1)
		printf("这是打日志的语句!\n");

	return;
}
