# Indexer

TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

## Usage

`./indexer pageDirectory indexFilename`

where `pageDirectory` is the pathname of a directory produced by the Crawler and `indexFilename` is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).

## Compilation and Testing

Command to compile both `indexer` and `indextest` : `make`

Command to clean : `make clean`

Command to run test : `make test &> testing.out`