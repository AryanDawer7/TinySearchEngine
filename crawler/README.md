# Crawler
This specific crawler module is a program designed to 
scrape and store web pages from the Internet, implemented in one file called crawler.c. 
The program uses two data structures, a 'bag' of pages that need to be crawled and 
a 'hashtable' of URLs that have been seen during the crawl. 

## Usage
`./crawler seedURL pageDirectory maxDepth`
where `seedURL` is the website to crawl, `pageDirectory` is an existing directory to store pages in, and `maxDepth` is the number of levels deep to crawl the seedURL.

## Compilation and Testing
Command to compile : `make`


Command to clean : `make clean`


Command to run test : `make test &> testing.out`

`make test` creates a testing.out which includes all unit tests. The requirements of the assignment 
stated the need to test the following: 
* "Required tests": (valgrind with letters at depths 0,10, toscrape at depths 0,1, wikipedia at depths 0,1).
I have included all "required tests" in a seperate testing script valgrind.sh and output file valgrind.out.

Command to run memory test (valgrind): `make valgrind &> valgrind.out`

### Test types:
* no arguments (too few)
* external link
* invalid depths 
* non-existent directory
* too many args
* valid server with nonexistent URL
* valgrind test
* valid test with depth 0
* valid test with depth 1
* valid test with depth 2
* valid test with depth 3

## Exit Status
* 0: Successful Run
* 1: Invalid number of arguments/ Couldn't process arguments
* 2: External URL
* 3: Non-integer `maxDepth`
* 4: `maxDepth` not in range [1,10]
* 5: `pageDirectory` doesn't exist/ didn't open
* 6: Couldn't create hashtable
* 7: Couldn't create bag
* 8: Couldnt create webpage from `seedURL`
* 9: Invalid webpage
* 99: mem_malloc_assert() couldn't allocate memory

## Additional Infromation and Clarifications
* As mentioned about logging our webpage handling activity, including logs in testing.out file can make then very long. I have included logging printlines as commented code in crawler.c. The following is an example of its working with the line: `./crawler HTTP://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters10 10`

``` bash
Depth: 0    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 0    Added: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 1    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/A.html
Depth: 1    Added: http://cs50tse.cs.dartmouth.edu/tse/letters/A.html
Depth: 2    Found: https://en.wikipedia.org/wiki/Algorithm
Depth: 2    Ignored: https://en.wikipedia.org/wiki/Algorithm
Depth: 2    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/B.html
Depth: 2    Added: http://cs50tse.cs.dartmouth.edu/tse/letters/B.html
Depth: 2    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 2    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 3    Found: https://en.wikipedia.org/wiki/Breadth-first_search
Depth: 3    Ignored: https://en.wikipedia.org/wiki/Breadth-first_search
Depth: 3    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/C.html
Depth: 3    Added: http://cs50tse.cs.dartmouth.edu/tse/letters/C.html
Depth: 3    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/D.html
Depth: 3    Added: http://cs50tse.cs.dartmouth.edu/tse/letters/D.html
Depth: 3    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/E.html
Depth: 3    Added: http://cs50tse.cs.dartmouth.edu/tse/letters/E.html
Depth: 3    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 3    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 4    Found: https://en.wikipedia.org/wiki/ENIAC
Depth: 4    Ignored: https://en.wikipedia.org/wiki/ENIAC
Depth: 4    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/F.html
Depth: 4    Added: http://cs50tse.cs.dartmouth.edu/tse/letters/F.html
Depth: 4    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/G.html
Depth: 4    Added: http://cs50tse.cs.dartmouth.edu/tse/letters/G.html
Depth: 4    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/A.html
Depth: 4    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/A.html
Depth: 4    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 4    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 5    Found: https://en.wikipedia.org/wiki/Graph_traversal
Depth: 5    Ignored: https://en.wikipedia.org/wiki/Graph_traversal
Depth: 5    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/H.html
Depth: 5    Added: http://cs50tse.cs.dartmouth.edu/tse/letters/H.html
Depth: 5    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 5    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 6    Found: https://en.wikipedia.org/wiki/Huffman_coding
Depth: 6    Ignored: https://en.wikipedia.org/wiki/Huffman_coding
Depth: 6    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/B.html
Depth: 6    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/B.html
Depth: 6    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 6    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 5    Found: https://en.wikipedia.org/wiki/Fast_Fourier_transform
Depth: 5    Ignored: https://en.wikipedia.org/wiki/Fast_Fourier_transform
Depth: 5    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/H.html
Depth: 5    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/H.html
Depth: 5    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 5    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 4    Found: https://en.wikipedia.org/wiki/Depth-first_search
Depth: 4    Ignored: https://en.wikipedia.org/wiki/Depth-first_search
Depth: 4    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 4    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 4    Found: https://en.wikipedia.org/wiki/Computational_biology
Depth: 4    Ignored: https://en.wikipedia.org/wiki/Computational_biology
Depth: 4    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/D.html
Depth: 4    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/D.html
Depth: 4    Found: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
Depth: 4    Ignored: http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
```

* Implementation choice: If user enters a double (instead of integer), I exit with stderr that `maxDepth` must be an integer