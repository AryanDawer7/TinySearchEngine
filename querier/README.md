```
****************
* EXTENSION USED
****************
```

# Indexer

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

#### I have implemented the full querier according to the specs

## Usage

`./querier pageDirectory indexFilename`

where `pageDirectory` is the pathname of a directory produced by the Crawler and `indexFilename` is the pathname of a file into which the index should be read from.

## Compilation and Testing

Command to compile `querier` : `make`

Command to clean : `make clean`

Command to run test : `make test &> testing.out`