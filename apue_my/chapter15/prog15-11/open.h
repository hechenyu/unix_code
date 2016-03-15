/* 程序15-11 open.h头文件 */
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int recv_fd(int, ssize_t (*func)(int, const void *, size_t));
int send_fd(int, int);
int send_err(int, int, const char *);
