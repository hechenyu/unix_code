# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE #-D__USE_UNIX98
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	test test1 testcancel incr_rwlock1 test2 test3 test4

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

test:	test.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test1:	test1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

testcancel:	testcancel.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_rwlock1:	incr_rwlock1.o ../lib/timing.o ../lib/tv_sub.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test2:	test2.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test3:	test3.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test4:	test4.o ../lib/gf_time.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

