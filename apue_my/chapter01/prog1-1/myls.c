/* 程序1-1 列出一个目录中的所有文件 */
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *dirp;

	if (argc != 2) {
		fprintf(stderr, "usage: %s directory_name", argv[0]);
		exit(1);
	}
	if ((dp = opendir(argv[1])) == NULL) {
		fprintf(stderr, "can't open %s", argv[1]);
		exit(1);
	}

	while ((dirp = readdir(dp)) != NULL)
		printf("%s\n", dirp->d_name);

	closedir(dp);
	exit(0);
}

