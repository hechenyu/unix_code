# Makefile

RM = rm -f
CC = gcc
CFLAGS = -Wall -g
INCLUDE = 
LDFLAGS = -lpthread
LDPATH = 

PROGS =	client clientrst \
		serv01 serv02 serv03 serv04 serv05 serv06 serv07 serv08

.PHONY: all
all: $(PROGS)
	@echo "build OK!"

clean:
	@$(RM) $(PROGS) *.o
	@echo "clean OK!"

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(INCLUDE)

# The client to test the various servers.
client:	client.o pr_cpu_time.o ../lib/readn.o ../lib/tcp_connect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# A special client that sends an RST occasionally.
# Used to test the XTI server (should receive disconnect).
clientrst:	clientrst.o pr_cpu_time.o ../lib/readn.o ../lib/tcp_connect.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv00: traditional concurrent server: use as base level
serv00:	serv00.o web_child.o pr_cpu_time.o ../lib/readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv01: one fork per client (traditional concurrent server).
serv01:	serv01.o web_child.o sig_chld_waitpid.o pr_cpu_time.o ../lib/readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv02: prefork, no locking; works on BSD-derived systems
#	but not on SVR4-derived systems.
serv02:	serv02.o child02.o web_child.o pr_cpu_time.o ../lib/readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv02l: prefork, no locking, block in select instead of accept to see
#	select collisions; works on BSD-derived systems but not on SVR4.
serv02l:serv02.o child02l.o web_child.o pr_cpu_time.o ../lib/readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv02m: prefork, no locking; works on BSD-derived systems.
#	This version is "metered" to see #clients/child serviced.
serv02m:serv02m.o child02m.o web_child.o pr_cpu_time.o meter.o ../lib/readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv03: prefork, file locking using fcntl().  Similar to Apache server.
serv03:	serv03.o child03.o lock_fcntl.o web_child.o pr_cpu_time.o ../lib/readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv03m: prefork, file locking using fcntl(), metered.
serv03m:	serv03m.o child03m.o lock_fcntl.o web_child.o pr_cpu_time.o meter.o ../lib/readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv04: prefork, file locking using pthread locking.
serv04:	serv04.o child04.o lock_pthread.o web_child.o pr_cpu_time.o ../lib/readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv05: prefork, descrptor passing to children.  Similar to NSCA server.
serv05:	serv05.o child05.o lock_fcntl.o web_child.o pr_cpu_time.o ../lib/readline.o ../lib/tcp_listen.o ../lib/writen.o ../lib/read_fd.o ../lib/write_fd.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# Thread versions must call a reentrant version of readline().
# serv06: one thread per client.
serv06:	serv06.o web_child.o pr_cpu_time.o readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv07: prethread with mutex locking around accept().
serv07:	serv07.o pthread07.o web_child.o pr_cpu_time.o readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv08: prethread with only main thread doing accept().
serv08:	serv08.o pthread08.o web_child.o pr_cpu_time.o readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

# serv09: prethread with no locking around accept().
serv09:	serv09.o pthread09.o web_child.o pr_cpu_time.o readline.o ../lib/tcp_listen.o ../lib/writen.o
	$(CC) -o $@ $^ $(LDFLAGS) $(LDPATH)

