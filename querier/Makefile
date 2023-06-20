# Makefile for CS50 "querier" library
#
# Aryan Dawer, May 2023

PROG = querier
OBJS = querier.o
LIBS = ../libcs50/libcs50-given.a ../common/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50
CC = gcc
MAKE = make

.PHONY: test clean

querier: querier.o $(LIBS)
	$(CC) $(CFLAGS) querier.o ../common/index.o ../common/pagedir.o $(LIBS) -o querier

querier.o: ../libcs50/webpage.h ../libcs50/counters.h ../libcs50/hashtable.h ../common/word.h ../libcs50/file.h ../libcs50/mem.h ../common/pagedir.h ../common/index.h

test:
	bash -v testing.sh

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f querier