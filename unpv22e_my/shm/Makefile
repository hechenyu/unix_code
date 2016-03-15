# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE #-D__USE_UNIX98
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 


# incr_map_anon and incr_dev_zero are not in PORGS since they're
# not on all platforms.

PROGS =	incr1 incr2 incr2private incr3 svmsgread \
		test1 test2 test3 incr_dev_zero incr_map_anon

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

incr1:	incr1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr2:	incr2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr2private:	incr2private.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr3:	incr3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_dev_zero:	incr_dev_zero.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

incr_map_anon:	incr_map_anon.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

svmsgread:	svmsgread.o ../lib/my_shm.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test1:	test1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test2:	test2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test3:	test3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

