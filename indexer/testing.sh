#!bin/bash
#
# testing.sh - script running tests 'indexer' module
#
# Outputs the result of each test
#

# Run integration test for the indexer

##### Invalid Tests

# 1. Test indexer with various invalid arguments.
./indexer test.txt s3e3
./indexer 2 3

# 2. no arguments.
./indexer

# 3. one argument.
./indexer ../data/toscrape

# 4. three or more arguments.
./indexer ../data/toscrape file test tester

# 5. invalid pageDirectory (non-existent path).
./indexer ../hello ../data/test1.index

# 6. invalid pageDirectory (not a crawler directory).
mkdir nonCrawlerDir

./indexer nonCrawlerDir ../data/test1.index

rmdir nonCrawlerDir

# 7. invalid indexFile (non-existent path)
./indexer ../data/toscrape ../data/nonexistingpath/index.test

# 8. invalid indexFile (read-only directory)
mkdir ../data/readOnlyDir
chmod -R a-w ../data/readOnlyDir

./indexer ../data/toscrape ../data/readonly_dir/toscrape.index

rmdir ../data/readOnlyDir

# 9. invalid indexFile (existing, read-only file)
touch ../data/readOnly.index
chmod a-w ../data/readOnly.index

./indexer ../data/toscrape ../data/readOnly.index

rm -f ../data/readOnly.index

##### Valid Tests with Valgrind

## "toscrape" directory created by crawler at depth 1

# Create toscrape with crawler
mkdir ../data/toscrape
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 1

# Valid Test for indexer (Create index file for crawler's toscrape depth 1) with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/toscrape ../data/toscrape.index

# Valid test for indextest (Validate indexer output) with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./indextest ../data/toscrape.index ../data/toscrapeTest.index

# Valid test for indexcmp (Check if created index is correct)
~/cs50-dev/shared/tse/indexcmp ../data/toscrape.index /thayerfs/home/f006gsd/cs50-dev/shared/tse/indices/toscrape-1.ndx

# Valid test for indextest (Check if created indextest.c produced same outputFile as inputfile)
~/cs50-dev/shared/tse/indexcmp ../data/toscrape.index ../data/toscrapeTest.index

# should be 0
echo $?

## "wikipedia" directory created by crawler at depth 1

# Create wikipedia with crawler
mkdir ../data/wikipedia
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia 1

# Valid Test for indexer (Create index file for crawler's toscrape depth 1) with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/wikipedia ../data/wikipedia.index

# Valid test for indextest (Validate indexer output) with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./indextest ../data/wikipedia.index ../data/wikipediaTest.index

# Valid test for indextest (Check if created indextest.c produced same outputFile as inputfile)
~/cs50-dev/shared/tse/indexcmp ../data/wikipedia.index ../data/wikipediaTest.index

# should be 0
echo $?