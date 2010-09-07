CFLAGS=-g -Wall -Werror

all: pf

pf: pf.o prog.o

pf.o: prefix.h
prog.o: prefix.h

prog.c: prog.pf c.py
	python c.py < prog.pf > prog.c || (rm prog.c; false)


