# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -DDEBUG
INCLUDE = 
LDFLAGS = -lpthread
LDPATH = 

PROGS =	locknone lockfcntl lockopen locknoneubuf lockpxsem locksvsem \
		locklink lockopen2 locksvsemrace1 \
		loopnone loopfcntl loopopen loopnonenonb looppxsem loopsvsem \
		loopfcntlnonb onedaemon \
		test1 test2 test3 test4

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

locknone: lockmain.o locknone.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lockfcntl: lockmain.o lockfcntl.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lockopen: lockmain.o lockopen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lockopen2: lockmain.o lockopen2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

locknoneubuf: lockmainubuf.o locknone.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

lockpxsem: lockmain.o lockpxsem.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

locksvsem: lockmain.o locksvsem.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

locksvsemrace1:	locksvsemrace1.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

locklink: lockmain.o locklink.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

loopnone: loopmain.o locknone.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

loopfcntl: loopmain.o lockfcntl.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

loopopen: loopmain.o lockopen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

loopnonenonb: loopmainnonb.o locknone.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

loopfcntlnonb: loopmainnonb.o lockfcntl.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

looppxsem: loopmain.o lockpxsem.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

loopsvsem: loopmain.o locksvsem.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

onedaemon: onedaemon.o ../lib/lock_reg.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test1: test1.o ../lib/lock_reg.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test2: test2.o ../lib/lock_reg.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test3: test3.o ../lib/lock_reg.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test4: test4.o ../lib/lock_reg.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

