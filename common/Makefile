# Makefile for CS50 library
#   Builds and compiles all these utility functions into a library.
#
# Aryan Dawer, April 2023

# object files, and the target library
OBJS = pagedir.o index.o word.o
LIB = common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS)
CC = gcc
MAKE = make

# Built by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h
index.o: index.h
word.o: word.h

.PHONY: clean

# clean up after our compilation
clean:
	rm -f $(LIB) *~ *.o
	rm -rf *.dSYM
