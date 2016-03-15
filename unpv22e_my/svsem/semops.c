#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char **argv)
{
	int		c, i, flag, semid, nops;
	struct sembuf	*ptr;
	key_t key;

	flag = 0;
	while ((c = getopt(argc, argv, "nu")) != -1) {
		switch (c) {
		case 'n':
			flag |= IPC_NOWAIT;		/* for each operation */
			break;

		case 'u':
			flag |= SEM_UNDO;		/* for each operation */
			break;

		case '?':
			exit(1);
		}
	}
	if (argc - optind < 2) {			/* argc - optind = #args remaining */
		fprintf(stderr, "usage: semops [ -n ] [ -u ] <pathname> operation ...\n");
		exit(1);
	}

	if ((key = ftok(argv[optind], 0)) == -1) {
		fprintf(stderr, "ftok error for pathname \"%s\" and id %d: %s\n",
			argv[optind], 0, strerror(errno));
		exit(1);
	}
	if ((semid = semget(key, 0, 0)) == -1) {
		perror("semget error");
		exit(1);
	}
	optind++;
	nops = argc - optind;

		/* 4allocate memory to hold operations, store, and perform */
	if ((ptr = calloc(nops, sizeof(struct sembuf))) == NULL) {
		perror("calloc error");
		exit(1);
	}
	for (i = 0; i < nops; i++) {
		ptr[i].sem_num = i;
		if (argv[optind+i][0] == '0') {
			ptr[i].sem_op = atoi(&argv[optind + i][1]);	/* <0, 0, or >0 */
		} else {
			ptr[i].sem_op = atoi(argv[optind + i]);	/* <0, 0, or >0 */
		}
		ptr[i].sem_flg = flag;
	}
	if (semop(semid, ptr, nops) == -1) {
		perror("semop error");
		exit(1);
	}

	exit(0);
}
