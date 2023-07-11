# Tiny Search Engine

Tiny Search Engine project was created for Dartmouth CS50 class in Spring 2023. The project is divided into three components as follows:
1. Crawler: Crawls through a website and retrieves webpages up until a given `depth`, and extracts the embedded URLs.
2. Indexer: Builds an inverted index from the crawler directory, mapping from words to document ID and count.
3. Querier: Returns webpages based on the user's queries (supports 'and' and 'or' operators).

Please see individual directories for more information.

To build, run `make`

To test, run `make test`

To clean up, run `make clean`

CS50 Playground: [http://cs50tse.cs.dartmouth.edu/tse/](http://cs50tse.cs.dartmouth.edu/tse/)
