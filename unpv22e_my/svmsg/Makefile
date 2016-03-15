# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	ctl limits slot slotseq twoqueues testumask \
		msgcreate msgsnd msgrcv msgrcvid msgrmid

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

msgcreate:	msgcreate.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

msgsnd:	msgsnd.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

msgrcv:	msgrcv.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

msgrcvid:	msgrcvid.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

msgrmid:	msgrmid.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

ctl:	ctl.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

limits:	limits.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

slot:	slot.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

slotseq:	slotseq.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

twoqueues:	twoqueues.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

testumask:	testumask.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

