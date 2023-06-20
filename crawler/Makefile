# Makefile for CS50 "crawler" library
#
# Aryan Dawer, April 2023

PROG = crawler
OBJS = crawler.o 
LIBS = ../libcs50/libcs50-given.a ../common/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../common -I../libcs50
CC = gcc
MAKE = make

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(PROG)

crawler.o: ../libcs50/bag.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/mem.h ../common/pagedir.h

.PHONY: test clean

test:
	bash -v testing.sh

valgrind: 
	bash -v valgrind.sh

clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f $(PROG)
	rm -rf ../data/hello ../data/letters0 ../data/letters1 ../data/letters2 ../data/letters3 ../data/toscrape
	rm -rf ../data