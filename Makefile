#############################################
# 
# Simple makefile for simple alarm
#
INSTALL=install
WHERE=~/bin
#IDIR =../include
CC=gcc
#CFLAGS=-I$(IDIR)
#CFLAGS=-I.
CFLAGS=
LDFLAGS=
DEPS =
OBJS = alarm.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: alarm

alarm: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ core  

cleanall:
	rm -f *.o *~ core alarm

install: alarm
	$(INSTALL) -s alarm $(WHERE)

