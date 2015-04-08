#############################################
# 
# Simple makefile for simple alarm
#
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

install:
	install -s alarm /usr/local/sbin

