/* DEBUG的新途径 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *pstr;

	if ((pstr = getenv("DEBUGON")) == NULL)
		;
	else
		printf("这是打日志的语句!\n");

	return;
}
