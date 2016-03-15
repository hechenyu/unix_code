# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE #-D__USE_UNIX98
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	semcreate semgetvalue semwait sempost semunlink \
		prodcons1 prodcons2 prodcons3 prodcons4 \
		deadlock semsysconf testeintr \
		mycat1 mycat2

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

semcreate: semcreate.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

semgetvalue: semgetvalue.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

semwait: semwait.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

sempost: sempost.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

semunlink: semunlink.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons1: prodcons1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons2: prodcons2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons3: prodcons3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons4: prodcons4.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mycat1: mycat1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mycat2: mycat2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

deadlock: deadlock.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

semsysconf: semsysconf.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

testeintr: testeintr.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

