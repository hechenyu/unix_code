#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *
gf_time(void);

void *sleep_nsec(void *arg)
{
	int n = (int) arg;

	long this_id = pthread_self();
	fprintf(stderr, "thread %ld sleep %d secs begin at: %s\n", this_id, n, gf_time());
	sleep(n);
	fprintf(stderr, "thread %ld sleep %d secs return at: %s\n", this_id, n, gf_time());

	return NULL;
}

int main()
{
	pthread_t tid1, tid2;
	int ret;
	int main_sleep_sec = 2;
	int thr1_sleep_sec = 5;
	int thr2_sleep_sec = 7;

	// 创建线程1, 并让线程1睡眠5秒
	if ((ret = pthread_create(&tid1, NULL, sleep_nsec, (void *) thr1_sleep_sec)) != 0) {
		perror("pthread_create error");
		exit(1);
	}

	fprintf(stderr, "create thread %ld ok at: %s\n", (long) tid1, gf_time());

	// 主线程睡眠2秒
	fprintf(stderr, "main thread sleep %d secs begin at: %s\n", main_sleep_sec, gf_time());
	sleep(main_sleep_sec);
	fprintf(stderr, "main thread sleep %d secs return at: %s\n", main_sleep_sec, gf_time());
	
	// 创建线程2, 并让线程2睡眠7秒
	if ((ret = pthread_create(&tid2, NULL, sleep_nsec, (void *) thr2_sleep_sec)) != 0) {
		perror("pthread_create error");
		exit(1);
	}

	fprintf(stderr, "create thread %ld ok at: %s\n", (long) tid2, gf_time());

	printf("wait for any char to exit\n");
	scanf("%d", &ret);

	return 0;
}

