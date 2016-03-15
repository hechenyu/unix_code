/* 程序4-7 递归降序遍历目录层次结构, 按文件类型计数 */
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

typedef int Myfunc(const char *, const struct stat *, int);
		/* function type that's called for each filename */

static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

int main(int argc, char *argv[])
{
	int ret;

	if (argc != 2) {
		perror("usage: ftw <starting-pathname>\n");
		exit(1);
	}

	ret = myftw(argv[1], myfunc);	/* dose it all */

	if ((ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock) == 0)
		ntot = 1;		/* avoid divide by 0; print 0 for all counts */
	printf("regular files = %7ld, %5.2f %%\n", nreg, nreg*100.0/ntot);
	printf("directories = %7ld, %5.2f %%\n", ndir, ndir*100.0/ntot);
	printf("block special = %7ld, %5.2f %%\n", nblk, nblk*100.0/ntot);
	printf("char special = %7ld, %5.2f %%\n", nchr, nchr*100.0/ntot);
	printf("FIFOs = %7ld, %5.2f %%\n", nfifo, nfifo*100.0/ntot);
	printf("symbolic links = %7ld, %5.2f %%\n", nslink, nslink*100.0/ntot);
	printf("sockets = %7ld, %5.2f %%\n", nsock, nsock*100.0/ntot);

	exit(ret);
}

/*
 * Descend through the hierarchy, starting at "pathname".
 * The caller's func() is called for every file.
 */

#define	FTW_F	1		/* file other than directory */
#define	FTW_D	2		/* directory */
#define	FTW_DNR	3		/* directory that can't be read */
#define	FTW_NS	4		/* file that we can't stat */

static char *fullpath;		/* contains full pathname for every file */

static int myftw(char *pathname, Myfunc *func)
{
	char *path_alloc(int *);	

	fullpath = path_alloc(NULL);	/* malloc's for PATH_MAX+1 bytes */
	strcpy(fullpath, pathname);		/* initialize fullpath */

	return((dopath(func)));
}

/* 
 * Descend through the hierarchy, starting at "fullpath".
 * If "fullpath" is anything other than a directory, we lstat() it,
 * call func(), and return. For a directory, we call ourself
 * recursively for each name in the directory.
 */
static int dopath(Myfunc *func)
{
	struct stat statbuf;
	struct dirent *dirp;
	DIR *dp;
	int ret;
	char *ptr;

	if (lstat(fullpath, &statbuf) < 0)
		return(func(fullpath, &statbuf, FTW_NS));		/* stat error */

	if (S_ISDIR(statbuf.st_mode) == 0)
		return(func(fullpath, &statbuf, FTW_F));		/* not a directory */

	/*
	 * It's a directory. First call func() for the directory,
	 * then process each filename in the directory.
	 */
	if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
		return(ret);

	ptr = fullpath + strlen(fullpath);		/* point to end of fullpath */
	*ptr++ = '/';
	*ptr = 0;

	if ((dp = opendir(fullpath)) == NULL)	/* can't read directory */
		return(func(fullpath, &statbuf, FTW_DNR));

	while ((dirp = readdir(dp)) != NULL) {
		if (strcmp(dirp->d_name, ".") == 0 ||
			strcmp(dirp->d_name, "..") == 0)
				continue;		/* ignore dot and dot-dot */

		strcpy(ptr, dirp->d_name);		/* append name after slash */

		if ((ret = dopath(func)) != 0)	/* recursive */
			break;		/* time to leave */
	}
	ptr[-1] = 0;	/* erase everything from slash onwards */

	if (closedir(dp) < 0)
		fprintf(stderr, "can't close directory %s\n", fullpath);

	return(ret);
}

static int myfunc(const char *pathname, const struct stat *statptr, int type)
{
	switch (type) {
	case FTW_F:
		switch (statptr->st_mode & S_IFMT) {
		case S_IFREG:
			nreg++;
			break;
		case S_IFBLK:
			nblk++;
			break;
		case S_IFCHR:
			nchr++;
			break;
		case S_IFIFO:
			nfifo++;
			break;
		case S_IFLNK:
			nslink++;
			break;
		case S_IFSOCK:
			nsock++;
			break;
		case S_IFDIR:	/* directories should have type = FTW_D */
			fprintf(stderr, "for S_IFDIR for %s\n", pathname);
			exit(1);
		}
		break;
	case FTW_D:
		ndir++;
		break;
	case FTW_DNR:
		fprintf(stderr, "can't read directory %s\n", pathname);
		break;
	case FTW_NS:
		fprintf(stderr, "stat error for %s\n", pathname);
		break;
	default:
		fprintf(stderr, "unknown type %d for pathname %s\n", type, pathname);
		exit(1);
	}

	return(0);
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
				perror("pathconf error for _PC_PATH_MAX\n");
				exit(1);
			}
		} else
			pathmax++;		/* add one since it's relative to root */
	}

	if ((ptr = malloc(pathmax+1)) == NULL) {
		perror("malloc error for pathname\n");
		exit(1);
	}

	if (size != NULL)
		*size = pathmax + 1;
	return(ptr);
}

