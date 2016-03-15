/* 程序2-1 打印所有可能的sysconf和pathconf值 */
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

static void pr_sysconf(char *, int);
static void pr_pathconf(char *, char *, int);

int main(int argc, char *argv[])
{
	if (argc != 2) {
		perror("usage: a.out <dirname>");
		exit(1);
	}

	pr_sysconf("ARG_MAX\t\t\t=", _SC_ARG_MAX);	/* exec函数的参数最大长度(Byte) */
	pr_sysconf("CHILD_MAX\t\t=", _SC_CHILD_MAX);	/* 每个实际用户ID的最大进程数 */
	pr_sysconf("clock ticks/second\t=", _SC_CLK_TCK);	/* 每秒时钟滴答数 */
	pr_sysconf("NGROUPS_MAX\t\t=", _SC_NGROUPS_MAX);	/* 每个进程的最大同时添加组ID数 */
	pr_sysconf("OPEN_MAX\t\t=", _SC_OPEN_MAX);	/* 每个进程最大打开文件数 */
#ifdef _SC_STREAM_MAX
	pr_sysconf("STREAM_MAX\t\t=", _SC_STREAM_MAX);	/* 在任一时刻每个进程的最大标准I/O流数 */
#endif
#ifdef _SC_TZNAME_MAX
	pr_sysconf("TZNAME_MAX\t\t=", _SC_TZNAME_MAX);	/* 时区名中的最大字节数 */
#endif
	pr_sysconf("_POSIX_JOB_CONTROL\t=", _SC_JOB_CONTROL);	/* 指明实现是否支持作业控制 */
	pr_sysconf("_POSIX_SAVED_IDS\t=", _SC_SAVED_IDS);	 /* 指明实现是否支持保存的设置-用户-ID和保存的设置-组-ID */
	pr_sysconf("_POSIX_VERSION\t\t=", _SC_VERSION);		/* 指明POSIX.1版本 */
	pr_pathconf("MAX_CANON\t\t=", "/dev/tty", _PC_MAX_CANON);/* 在一终端规范输入队列的最大字节数 */
	pr_pathconf("MAX_INPUT\t\t=", "/dev/tty", _PC_MAX_INPUT);/* 终端输入队列可用空间的字节数 */
	pr_pathconf("_POSIX_VDISABLE\t\t=", "/dev/tty", _PC_VDISABLE);/* 若定义,终端专用字符可用此值限制 */
	pr_pathconf("LINK_MAX\t\t=", argv[1], _PC_LINK_MAX);	/* 文件连接数的最大值 */ 
	pr_pathconf("NAME_MAX\t\t=", argv[1], _PC_NAME_MAX); /* 文件名中的最大字节数(不包括'\0') */
	pr_pathconf("PATH_MAX\t\t=", argv[1], _PC_PATH_MAX); /* 相对路径名的最大字节数(不包括'\0') */
	pr_pathconf("PIPE_BUF\t\t=", argv[1], _PC_PIPE_BUF); /* 能原子地写到一管道的最大字节数 */
	pr_pathconf("_POSIX_NO_TRUNC\t\t=", argv[1], _PC_NO_TRUNC);  /* 指明若路径名长于NAME_MAX是否产生一错误 */ 
	pr_pathconf("_POSIX_CHOWN_RESTRICTED\t=", argv[1], _PC_CHOWN_RESTRICTED); /* 指明使用chown是否受到限制 */

	exit(0);
}

static void pr_sysconf(char *mesg, int name)
{
	long val;

	fputs(mesg, stdout);
	errno = 0;
	if ((val = sysconf(name)) < 0) {
		if (errno != 0) {
			perror("sysconf error");
			exit(1);
		}
		fputs(" (not defined)\n", stdout);
	} else
		printf(" %ld\n", val);
}

static void pr_pathconf(char *mesg, char *path, int name)
{
	long val;

	fputs(mesg, stdout);
	errno = 0;
	if ((val = pathconf(path, name)) < 0) {
		if (errno != 0) {
			fprintf(stderr, "pathconf error, path = %s", path);
			exit(1);
		}
		fputs(" (no limit)\n", stdout);
	} else
		printf(" %ld\n", val);
}
			
			
