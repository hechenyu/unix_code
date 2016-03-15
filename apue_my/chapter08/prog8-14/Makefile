# Makefile for "comline"

CC = cc
OBJ = comline.o pr_exit.o

tsys : $(OBJ)
	$(CC) $(OBJ) -o $@

comline.o : comline.c
	$(CC) -c $<

pr_exit.o : pr_exit.c
	$(CC) -c $<

