# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE #-D__USE_UNIX98
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	incr_pxmutex0 incr_pxmutex1 incr_pxmutex5 \
		incr_pxsem1 incr_pxsem2 incr_pxsem5 incr_pxsem6 \
		incr_pxsem7 incr_pxsem8 incr_pxsem9 \
		incr_svsem1 incr_svsem2 incr_svsem5 incr_svsem6 \
		incr_rwlock1 incr_fcntl1 incr_fcntl5 \
		bw_pipe bw_pxmsg bw_svmsg \
		lat_pipe lat_pxmsg lat_svmsg \
		lat_signal lat_sigwait lat_sigqueue

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

incr_pxmutex0:	incr_pxmutex0.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_pxmutex1:	incr_pxmutex1.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_pxmutex5:	incr_pxmutex5.o ../lib/timing.o ../lib/tv_sub.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_pxsem1:	incr_pxsem1.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_pxsem2:	incr_pxsem2.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_pxsem5:	incr_pxsem5.o ../lib/timing.o ../lib/tv_sub.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_pxsem6:	incr_pxsem6.o ../lib/timing.o ../lib/tv_sub.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_pxsem7:	incr_pxsem7.o ../lib/timing.o ../lib/tv_sub.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_pxsem8:	incr_pxsem8.o ../lib/timing.o ../lib/tv_sub.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_pxsem9:	incr_pxsem9.o ../lib/timing.o ../lib/tv_sub.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_svsem1:	incr_svsem1.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_svsem2:	incr_svsem2.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_svsem5:	incr_svsem5.o ../lib/timing.o ../lib/tv_sub.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_svsem6:	incr_svsem6.o ../lib/timing.o ../lib/tv_sub.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_rwlock1:	incr_rwlock1.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_fcntl1:	incr_fcntl1.o ../lib/timing.o ../lib/tv_sub.o ../lib/lock_reg.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_fcntl5:	incr_fcntl5.o ../lib/timing.o ../lib/tv_sub.o ../lib/my_shm.o ../lib/lock_reg.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

bw_pipe:	bw_pipe.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

bw_pxmsg:	bw_pxmsg.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

bw_svmsg:	bw_svmsg.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lat_pipe:	lat_pipe.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lat_pxmsg:	lat_pxmsg.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lat_svmsg:	lat_svmsg.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lat_signal:	lat_signal.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lat_sigwait:	lat_sigwait.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lat_sigqueue:	lat_sigqueue.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

