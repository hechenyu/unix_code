# Makefile

#-D_GNU_SOURCE to open macro __USE_UNIX98

RM = rm -f
CC = gcc
CFLAGS = -Wall -g -D_GNU_SOURCE 
INCLUDE = 
LDFLAGS = -lpthread -lrt
LDPATH = 

PROGS =	prodcons1 prodcons2 prodcons3 prodcons4 prodcons5 prodcons6 \
		prodcons7 test1 test2 test3 test4

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

prodcons1: prodcons1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons2: prodcons2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons3: prodcons3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons4: prodcons4.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons5: prodcons5.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons6: prodcons6.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

prodcons7: prodcons7.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test1: test1.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test2: test2.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test3: test3.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

test4: test4.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

