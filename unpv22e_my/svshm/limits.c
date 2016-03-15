#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define	SVSHM_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV shared memory */

#define	MAX_NIDS	4096

int
main(int argc, char **argv)
/* $$.bp$$ */
{
	int		i, j, shmid[MAX_NIDS];
	void	*addr[MAX_NIDS];
	unsigned long	size;

		/* 4see how many identifiers we can "open" */
	for (i = 0; i < MAX_NIDS; i++) {
		shmid[i] = shmget(IPC_PRIVATE, 1024, SVSHM_MODE | IPC_CREAT);
		if (shmid[i] == -1) {
			printf("%d identifiers open at once\n", i);
			break;
		}
	}
	for (j = 0; j < i; j++) {
		if (shmctl(shmid[j], IPC_RMID, NULL) == -1) {
			perror("shmctl error");
			exit(1);
		}
	}

		/* 4now see how many we can "attach" */
	for (i = 0; i < MAX_NIDS; i++) {
		if ((shmid[i] = shmget(IPC_PRIVATE, 1024, SVSHM_MODE | IPC_CREAT)) == -1) {
			perror("shmget error");
			exit(1);
		}
		addr[i] = shmat(shmid[i], NULL, 0);
		if (addr[i] == (void *) -1) {
			printf("%d shared memory segments attached at once\n", i);
			if (shmctl(shmid[i], IPC_RMID, NULL) == -1) {	/* the one that failed */
				perror("shmctl error");
				exit(1);
			}
			break;
		}
	}
	for (j = 0; j < i; j++) {
		if (shmdt(addr[j]) == -1) {
			perror("shmdt error");
			exit(1);
		}
		if (shmctl(shmid[j], IPC_RMID, NULL) == -1) {
			perror("shmctl error");
			exit(1);
		}
	}

		/* 4see how small a shared memory segment we can create */
	for (size = 1; ; size++) {
		shmid[0] = shmget(IPC_PRIVATE, size, SVSHM_MODE | IPC_CREAT);
		if (shmid[0] != -1) {		/* stop on first success */
			printf("minimum size of shared memory segment = %lu\n", size);
			if (shmctl(shmid[0], IPC_RMID, NULL) == -1) {
				perror("shmctl error");
				exit(1);
			}
			break;
		}
	}

		/* 4see how large a shared memory segment we can create */
	for (size = 65536; ; size += 4096) {
		shmid[0] = shmget(IPC_PRIVATE, size, SVSHM_MODE | IPC_CREAT);
		if (shmid[0] == -1) {		/* stop on first failure */
			printf("maximum size of shared memory segment = %lu\n", size-4096);
			break;
		}
		if (shmctl(shmid[0], IPC_RMID, NULL) == -1) {
			perror("shmctl error");
			exit(1);
		}
	}

	exit(0);
}
