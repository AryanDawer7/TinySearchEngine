# Makefile for CS50 "indexer" library
#
# Aryan Dawer, April 2023

PROG = indexer indextest
OBJS = indexer.o indextest.o
LIBS = ../libcs50/libcs50-given.a ../common/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50
CC = gcc
MAKE = make

.PHONY: all test clean

all: $(PROG)

indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) indexer.o $(LIBS) -o indexer

indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) indextest.o ../common/index.o $(LIBS) -o indextest

indexer.o: ../libcs50/counters.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/mem.h ../common/pagedir.h ../common/index.h

indextest.o: ../libcs50/counters.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/file.h ../libcs50/mem.h ../common/pagedir.h ../common/index.h

test:
	bash -v testing.sh

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f indexer
	rm -f indextest
