#!bin/bash
#
# testing.sh - script running tests 'crawler' module
#
# Outputs the result of each test
#

# make directories
mkdir ../data
mkdir ../data/hello ../data/letters0 ../data/letters1 ../data/letters2 ../data/letters3 ../data/toscrape

# make binary files
make

# no arguments (too few)
./crawler

# external link
./crawler http://www.google.com ../data/letters1 1

# invalid depths                                                                 
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/letters1 -1                                                                          
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/letters1 11 

# non-existent directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/nonExistentDir 1

# too many args
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/letters1 1 test

# valid server with nonexistent URL
./crawler http://cs50tse.cs.dartmouth.edu/tse/hello.html ../data/hello 2

#valgrind test
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 1

# valid test with depth 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters0 0

# valid test with depth 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters1 1

# valid test with depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters2 2

# valid test with depth 3
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters3 3