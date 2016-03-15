/* DEBUG的旧途径 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
#ifdef DEBUGON
	printf("这是打日志的语句!\n");
#endif

	return;
}
