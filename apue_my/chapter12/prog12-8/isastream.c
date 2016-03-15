/* 程序12-8 检查描述符是否引用流设备 */
#include <stropts.h>
#include <unistd.h>

int isastream(int fd)
{
	return(ioctl(fd, I_CANPUT, 0) != -1);
}
