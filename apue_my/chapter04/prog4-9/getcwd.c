/* ³ÌÐò4-9 getcwdº¯ÊýÊµÀý */
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

char *path_alloc(int *);

int main(void)
{
	char *ptr;
	int size;

	if (chdir("/usr/spool/uucppublic") < 0) {
		perror("chdir failed");
		exit(1);
	}

	ptr = path_alloc(&size);		/* our own function */
	if (getcwd(ptr, size) == NULL) {
		perror("getcwd failed");
		exit(1);
	}

	printf("cwd = %s\n", ptr);
	exit(0);
}

#ifdef PATH_MAX
static int pathmax = PATH_MAX;
#else
static int pathmax = 0;
#endif

#define PATH_MAX_GUESS 1024		/* if PATH_MAX is indeterminate */
							/* we're not guaranteed this is adequate */

char *path_alloc(int *size)		/* also return allocated size, if nonnull */
{
	char *ptr;

	if (pathmax == 0) {		/* first time through */
		errno = 0;
		if ((pathmax = pathconf("/", _PC_PATH_MAX)) < 0) {
			if (errno == 0)
				pathmax = PATH_MAX_GUESS;	/* it's indeterminate */
			else {
				perror("pathconf error for _PC_PATH_MAX");
				exit(1);
			}
		} else
			pathmax++;		/* add one since it's relative to root */
	}

	if ((ptr = malloc(pathmax+1)) == NULL) {
		perror("malloc error for pathname");
		exit(1);
	}

	if (size != NULL)
		*size = pathmax + 1;
	return(ptr);
}


