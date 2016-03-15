/* 程序3-5 对一个文件描述符关闭一个或多个文件状态标志 */
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void clr_fl(int fd, int flags)      /* flags are file status flags to turn off */
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        perror("fcntl F_GETFL error");
        exit(1);
    }

    val &= ~flags;  /* turn off flags */

    if (fcntl(fd, F_SETFL, val) < 0) {
        perror("fcntl F_SETFL error");
        exit(1);
    }
}

