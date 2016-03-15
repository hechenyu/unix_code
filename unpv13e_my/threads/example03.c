#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define	NLOOP	   	 50
#define	BUFFSIZE	 10

struct buf_t {
  int		b_buf[BUFFSIZE];	/* the buffer which contains integer items */
  int		b_nitems;			/* #items currently in buffer */
  int		b_nextget;
  int		b_nextput;
  pthread_mutex_t	b_mutex;
  pthread_cond_t	b_cond_consumer;	/* consumer waiting to get */
  pthread_cond_t	b_cond_producer;	/* producer waiting to put */
} buf_t;

void	*produce_loop(void *);
void	*consume_loop(void *);

int
main(int argc, char **argv)
{
	int			n;
	pthread_t	tidA, tidB;

	printf("main, addr(stack) = %x, addr(global) = %x, addr(func) = %x\n",
			&n, &buf_t, &produce_loop);
	if ( (n = pthread_create(&tidA, NULL, &produce_loop, NULL)) != 0) {
		errno = n;
		perror("pthread_create error for A");
		exit(1);
	}
	if ( (n = pthread_create(&tidB, NULL, &consume_loop, NULL)) != 0) {
		errno = n;
		perror("pthread_create error for B");
		exit(1);
	}

		/* wait for both threads to terminate */
	if ( (n = pthread_join(tidA, NULL)) != 0) {
		errno = n;
		perror("pthread_join error for A");
		exit(1);
	}
	if ( (n = pthread_join(tidB, NULL)) != 0) {
		errno = n;
		perror("pthread_join error for B");
		exit(1);
	}

	exit(0);
}

void
produce(struct buf_t *bptr, int val)
{
	int n;
	if ((n = pthread_mutex_lock(&bptr->b_mutex)) != 0) {
		errno = n;
		perror("pthread_mutex_lock error");
		exit(1);
	}
		/* Wait if buffer is full */
	while (bptr->b_nitems >= BUFFSIZE) {
		if ((n = pthread_cond_wait(&bptr->b_cond_producer, &bptr->b_mutex)) != 0) {
			errno = n;
			perror("pthread_cond_wait error");
			exit(1);
		}
	}

		/* There is room, store the new value */
	printf("produce %d\n", val);
	bptr->b_buf[bptr->b_nextput] = val;
	if (++bptr->b_nextput >= BUFFSIZE)
		bptr->b_nextput = 0;
	bptr->b_nitems++;

		/* Signal consumer */
	if ((n = pthread_cond_signal(&bptr->b_cond_consumer)) != 0) {
		errno = n;
		perror("pthread_cond_signal error");
		exit(1);
	}
	if ((n = pthread_mutex_unlock(&bptr->b_mutex)) != 0) {
		errno = n;
		perror("pthread_mutex_unlock error");
		exit(1);
	}
}

int
consume(struct buf_t *bptr)
{
	int		val;
	int		n;

	if ((n = pthread_mutex_lock(&bptr->b_mutex)) != 0) {
		errno = n;
		perror("pthread_mutex_lock error");
		exit(1);
	}
		/* Wait if buffer is empty */
	while (bptr->b_nitems <= 0) {
		if ((n = pthread_cond_wait(&bptr->b_cond_consumer, &bptr->b_mutex)) != 0) {
			errno = n;
			perror("pthread_cond_wait error");
			exit(1);
		}
	}

		/* There is data, fetch the value */
	val = bptr->b_buf[bptr->b_nextget];
	printf("consume %d\n", val);
	if (++bptr->b_nextget >= BUFFSIZE)
		bptr->b_nextget = 0;
	bptr->b_nitems--;

		/* Signal producer; it might be waiting for space to store */
	if ((n = pthread_cond_signal(&bptr->b_cond_producer)) != 0) {
		errno = n;
		perror("pthread_cond_signal error");
		exit(1);
	}
	if ((n = pthread_mutex_unlock(&bptr->b_mutex)) != 0) {
		errno = n;
		perror("pthread_mutex_unlock error");
		exit(1);
	}

	return(val);
}

void *
produce_loop(void *vptr)
{
	int		i;

	printf("produce_loop thread, addr(stack) = %x\n", &i);
	for (i = 0; i < NLOOP; i++) {
		produce(&buf_t, i);
	}

	return(NULL);
}

void *
consume_loop(void *vptr)
{
	int		i, val;

	printf("consume_loop thread, addr(stack) = %x\n", &i);
	for (i = 0; i < NLOOP; i++) {
		val = consume(&buf_t);
	}

	return(NULL);
}
