/* 程序14-11 打印不同类型的数据所存放的位置 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#undef SHM_SIZE
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W)		/* user read/write */

char array[ARRAY_SIZE];		/* uninitialized data = bss */

int main(void)
{
	int shmid;
	char *ptr, *shmptr;

	printf("array[] from %x to %x\n", &array[0], &array[ARRAY_SIZE]);
	printf("stack around %x\n", &shmid);

	if ((ptr = malloc(MALLOC_SIZE)) == NULL) {
		perror("malloc error");
		exit(1);
	}
	printf("malloced from %x to %x\n", ptr, ptr+MALLOC_SIZE);

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget error\n");
		exit(1);
	}
	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1) {
		perror("shmat error\n");
		exit(1);
	}
	printf("shared memory attached from %x to %x\n",
			shmptr, shmptr+SHM_SIZE);
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl error\n");
		exit(1);
	}

	exit(0);
}


