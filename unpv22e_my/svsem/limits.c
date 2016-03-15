#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "semun.h"

#define	SVSEM_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default permissions for new SV semaphores */

	/* 4following are upper limits of values to try */
#define	MAX_NIDS	4096		/* max # semaphore IDs */
#define	MAX_VALUE	1024*1024	/* max semaphore value */
#define	MAX_MEMBERS	4096		/* max # semaphores per semaphore set */
#define	MAX_NOPS	4096		/* max # operations per semop() */

int
main(int argc, char **argv)
{
	int		i, j, semid, sid[MAX_NIDS], pipefd[2];
	int		semmni, semvmx, semmsl, semmns, semopn, semaem, semume, semmnu;
	pid_t	*child;
	union semun	arg;
	struct sembuf	ops[MAX_NOPS];
	long MAX_NPROC = 0;

	if ((MAX_NPROC = sysconf(_SC_CHILD_MAX)) == -1) {
		if (errno != 0) {
			perror("sysconf error");
		} else {
			fprintf(stderr, "sysconf: %d not defined\n", _SC_CHILD_MAX);
		}
		exit(1);
	}

		/* 4see how many sets with one member we can create */
	for (i = 0; i <= MAX_NIDS; i++) {
		sid[i] = semget(IPC_PRIVATE, 1, SVSEM_MODE | IPC_CREAT);
		if (sid[i] == -1) {
			semmni = i;
			printf("%d identifiers open at once\n", semmni);
			break;
		}
	}
		/* 4before deleting, find maximum value using sid[0] */
	for (j = 7; j < MAX_VALUE; j += 8) {
		arg.val = j;
		if (semctl(sid[0], 0, SETVAL, arg) == -1) {
			semvmx = j - 8;
			printf("max semaphore value = %d\n", semvmx);
			break;
		}
	}
	for (j = 0; j < i; j++) {
		if (semctl(sid[j], 0, IPC_RMID) == -1) {
			perror("semctl error");
			exit(1);
		}
	}

		/* 4determine max # semaphores per semaphore set */
	for (i = 1; i <= MAX_MEMBERS; i++) {
		semid = semget(IPC_PRIVATE, i, SVSEM_MODE | IPC_CREAT);
		if (semid == -1) {
			semmsl = i-1;
			printf("max of %d members per set\n", semmsl);
			break;
		}
		if (semctl(semid, 0, IPC_RMID) == -1) {
			perror("semctl error");
			exit(1);
		}
	}

		/* 4find max of total # of semaphores we can create */
	semmns = 0;
	for (i = 0; i < semmni; i++) {
		sid[i] = semget(IPC_PRIVATE, semmsl, SVSEM_MODE | IPC_CREAT);
		if (sid[i] == -1) {
/* $$.bp$$ */
			/*
			 * Up to this point each set has been created with semmsl
			 * members.  But this just failed, so try recreating this
			 * final set with one fewer member per set, until it works.
			 */
			for (j = semmsl-1; j > 0; j--) {
				sid[i] = semget(IPC_PRIVATE, j, SVSEM_MODE | IPC_CREAT);
				if (sid[i] != -1) {
					semmns += j;
					printf("max of %d semaphores\n", semmns);
					if (semctl(sid[i], 0, IPC_RMID) == -1) {
						perror("semctl error");
						exit(1);
					}
					goto done;
				}
			}
			fprintf(stderr, "j reached 0, semmns = %d\n", semmns);
			exit(1);
		}
		semmns += semmsl;
	}
	printf("max of %d semaphores\n", semmns);
done:
	for (j = 0; j < i; j++)
		if (semctl(sid[j], 0, IPC_RMID) == -1) {
			perror("semctl error");
			exit(1);
		}

		/* 4see how many operations per semop() */
	if ((semid = semget(IPC_PRIVATE, semmsl, SVSEM_MODE | IPC_CREAT)) == -1) {
		perror("semget error");
		exit(1);
	}
	for (i = 1; i <= MAX_NOPS; i++) {
		ops[i-1].sem_num = i-1;
		ops[i-1].sem_op = 1;
		ops[i-1].sem_flg = 0;
		if (semop(semid, ops, i)  == -1) {
			if (errno != E2BIG) {
				perror("expected E2BIG from semop");
				exit(1);
			}
			semopn = i-1;
			printf("max of %d operations per semop()\n", semopn);
			break;
		}
	}
	if (semctl(semid, 0, IPC_RMID) == -1) {
		perror("semctl error");
		exit(1);
	}

		/* 4determine the max value of semadj */
		/* 4create one set with one semaphore */
	if ((semid = semget(IPC_PRIVATE, 1, SVSEM_MODE | IPC_CREAT)) == -1) {
		perror("semget error");
		exit(1);
	}
	arg.val = semvmx;
	if (semctl(semid, 0, SETVAL, arg) == -1) {		/* set value to max */
		perror("semctl error");
		exit(1);
	}
	for (i = semvmx-1; i > 0; i--) {
		ops[0].sem_num = 0;
		ops[0].sem_op = -i;
		ops[0].sem_flg = SEM_UNDO;
		if (semop(semid, ops, 1)  != -1) {
			semaem = i;
			printf("max value of adjust-on-exit = %d\n", semaem);
			break;
		}
	}
	if (semctl(semid, 0, IPC_RMID) == -1) {
		perror("semctl error");
		exit(1);
	}
/* $$.bp$$ */
		/* 4determine max # undo structures */
		/* 4create one set with one semaphore; init to 0 */
	if ((semid = semget(IPC_PRIVATE, 1, SVSEM_MODE | IPC_CREAT)) == -1) {
		perror("semget error");
		exit(1);
	}
	arg.val = 0;
	if (semctl(semid, 0, SETVAL, arg) == -1) {		/* set semaphore value to 0 */
		perror("semctl error");
		exit(1);
	}
	if (pipe(pipefd) == -1) {
		perror("pipe error");
		exit(1);
	}
	if ((child = malloc(MAX_NPROC * sizeof(pid_t))) == NULL) {
		perror("malloc error");
		exit(1);
	}
	for (i = 0; i < MAX_NPROC; i++) {
		if ( (child[i] = fork()) == -1) {
			semmnu = i - 1;
			printf("fork failed, semmnu at least %d\n", semmnu);
			break;
		} else if (child[i] == 0) {
			ops[0].sem_num = 0;			/* child does the semop() */
			ops[0].sem_op = 1;
			ops[0].sem_flg = SEM_UNDO;
			j = semop(semid, ops, 1);	/* 0 if OK, -1 if error */
			if (write(pipefd[1], &j, sizeof(j)) == sizeof(j)) {
				perror("write error");
				exit(1);
			}
			sleep(30);					/* wait to be killed by parent */
			exit(0);					/* just in case */
		}
		/* parent reads result of semop() */
		if (read(pipefd[0], &j, sizeof(j)) == -1) {
			perror("read error");
			exit(1);
		}
		if (j == -1) {
			semmnu = i;
			printf("max # undo structures = %d\n", semmnu);
			break;
		}
	}
	if (semctl(semid, 0, IPC_RMID) == -1) {
		perror("semctl error");
		exit(1);
	}
	for (j = 0; j <= i && child[j] > 0; j++) {
		if (kill(child[j], SIGINT) == -1) {
			perror("kill error");
			exit(1);
		}
	}

		/* 4determine max # adjust entries per process */
		/* 4create one set with max # of semaphores */
	if ((semid = semget(IPC_PRIVATE, semmsl, SVSEM_MODE | IPC_CREAT)) == -1) {
		perror("semget error");
		exit(1);
	}
	for (i = 0; i < semmsl; i++) {
		arg.val = 0;
		if (semctl(semid, i, SETVAL, arg) == -1) {		/* set semaphore value to 0 */
			perror("semctl error");
			exit(1);
		}

		ops[i].sem_num = i;
		ops[i].sem_op = 1;					/* add 1 to the value */
		ops[i].sem_flg = SEM_UNDO;
		if (semop(semid, ops, i+1) == -1) {
			semume = i;
			printf("max # undo entries per process = %d\n", semume);
			break;
		}
	}
	if (semctl(semid, 0, IPC_RMID) == -1) {
		perror("semctl error");
		exit(1);
	}

	exit(0);
}
