#!bin/bash
#
# testing.sh - script running tests 'querier' module
#
# Outputs the result of each test
#

# Setup
# mkdir ../data
# mkdir ../data/toscrape
# ../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 1
# ../indexer/indexer ../data/toscrape ../data/toscrape.index

# Testing for all sorts of invalid inputs (syntax/logic) given in the requirement spec
./querier ../data/toscrape ../data/toscrape.index < invalidQueries

# FuzzTesting with 10 queries and seed 11111
./querier ../data/toscrape ../data/toscrape.index < fuzzQueries

# Valgrind testing

# Test 1 (Invalid query)
valgrind --leak-check=full --show-leak-kinds=all echo "   above OR promise AND    " | ./querier ../data/toscrape ../data/toscrape.index

# Test 2 (Long valid query)
valgrind --leak-check=full --show-leak-kinds=all echo "angels OR race shrubby AND ago" | ./querier ../data/toscrape ../data/toscrape.index
