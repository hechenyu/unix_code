/* 程序5-4 tmpnam和tmpfile函数实例 */
#include <stdio.h>

#define	MAXLINE	4096

int main (void)
{
	char name[L_tmpnam], line[MAXLINE];
	FILE *fp;

	printf("%s\n", tmpnam(NULL));	/* first temp name */

	tmpnam(name);	/* second temp name */
	printf("%s\n", name);

	if ((fp = tmpfile()) == NULL) {	/* create temp file */
		perror("tmpfile error");
		exit(1);
	}
	fputs("one line of output\n", fp);	/* write to temp file */
	rewind(fp);		/* then read it back */
	if (fgets(line, sizeof(line), fp) == NULL) {
		perror("fgets error");
		exit(1);
	}
	fputs(line, stdout);	/* print the line we wrote */
	exit(0);
}
		
