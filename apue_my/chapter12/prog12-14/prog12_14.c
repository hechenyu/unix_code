/* 函数12-14 用存储映射I/O复制文件 */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>		/* mmap() */
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define     FILE_MODE       (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

#ifndef	MAP_FILE		/* 4.3+BSD defines this & requires it to mmap files */
#define	MAP_FILE	0	/* to compile under systems other than 4.3+BSD */
#endif

int main(int argc, char *argv[])
{
	int fdin, fdout;
	char *src, *dst;
	struct stat statbuf;

	if (argc != 3) {
		perror("usage: a.out <fromfile> <tofile>");
		exit(1);
	}

	if ((fdin = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "can't open %s for reading", argv[1]);
		exit(1);
	}

	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
		fprintf(stderr, "can't creat %s for writing", argv[2]);
		exit(1);
	}

	if (fstat(fdin, &statbuf) < 0) {		/* need size of input file */
		perror("fstat error");
		exit(1);
	}

			/* set size of output file */
	if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) == -1) {
		perror("lseek error");
		exit(1);
	}
	if (write(fdout, "", 1) != 1) {
		perror("write error");
		exit(1);
	}

	if ((src = mmap(0, statbuf.st_size, PROT_READ,
					MAP_FILE | MAP_SHARED, fdin, 0)) == (caddr_t) - 1) {
		perror("mmap error for input");		
		exit(1);
	}

	if ((src = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE,
					MAP_FILE | MAP_SHARED, fdout, 0)) == (caddr_t) - 1) {
		perror("mmap error for output");		
		exit(1);
	}

	memcpy(dst, src, statbuf.st_size);		/* does the file copy */

	exit(0);
}
