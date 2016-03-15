# my makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -c -g
INCLUDE =
LDFLAGS =
LDPATH =

TARGETS = readline.o signal.o timing.o tv_sub.o my_shm.o sleep_us.o \
	lock_reg.o lock_test.o gf_time.o signal_rt.o
LIBNAME = libmyunpv2.a

.PHONY: all
all: $(TARGETS)
#	g++ -shared -o libpacket.so $(TARGETS)
	ar r $(LIBNAME) $(TARGETS)

.PHONY: clean
clean:
	$(RM) $(TARGETS) $(LIBNAME)

$(TARGETS): %.o: %.c
	$(CC) $(INCLUDE) $(CFLAGS) $^ -o $@

