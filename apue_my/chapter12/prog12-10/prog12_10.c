/* 程序12-10 打印流中的模块名 */
#include <sys/conf.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stropts.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int isastream(int);

int main(int argc, char *argv[])
{
	int fd, i, nmods;
	struct str_list list;

	if (argc != 2) {
		perror("usage: a.out <pathname>");
		exit(1);
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "can't open %s", argv[1]);
		exit(1); 
	}
	if (isastream(fd) == 0) {
		fprintf(stderr, "%s is not a stream", argv[1]);
		exit(1);
	}

			/* fetch number of modules */
	if ((nmods = ioctl(fd, I_LIST, (void *) 0)) < 0) {
		perror("I_LIST error for nmods");
		exit(1);
	}
	printf("#modules = %d\n", nmods);

			/* allocate storage for all the module names */
	list.sl_modlist = calloc(nmods,  sizeof(struct str_mlist));
	if (list.sl_modlist == NULL) {
		perror("calloc error");
		exit(1);
	}
	list.sl_nmods = nmods;

			/* and fetch the module names */
	if (ioctl(fd, I_LIST, &list) < 0) {
		perror("I_LIST error for list");
		exit(1);
	}

			/* print the module nmaes */
	for (i = 1; i <= nmods; i++)
		printf("\t%s: %s\n", (i == nmods) ? "dirver" : "module",
				list.sl_modlist++);

	exit(0);
}

int isastream(int fd)
{
    return(ioctl(fd, I_CANPUT, 0) != -1);
}

