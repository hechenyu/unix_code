# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -DDEBUG 
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	mqcreate1 mqcreate mqgetattr \
		mqnotifysig1 mqnotifysig2 mqnotifysig3 mqnotifysig4 mqnotifysig5 \
		mqreceive mqsend mqsysconf mqunlink \
		test1 mqnotifythread1

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

mqcreate1: mqcreate1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqcreate: mqcreate.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqgetattr: mqgetattr.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqnotifysig1: mqnotifysig1.o ../lib/signal.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqnotifysig2: mqnotifysig2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqnotifysig3: mqnotifysig3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqnotifysig4: mqnotifysig4.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqnotifysig5: mqnotifysig5.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqnotifythread1: mqnotifythread1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH) -lpthread

mqreceive: mqreceive.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqsend: mqsend.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqsysconf: mqsysconf.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

mqunlink: mqunlink.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test1: test1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

