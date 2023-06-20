# CS50 TSE Indexer

## Implementation Spec
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan


## Data structures
We use 3 data structures (`set`, `hashtable`, and `counters`) and create 1 struct of our own `index`. The `index` struct (created with several methods to help indexer in the location `../common/index.c`) contains a hashtable with setpairs with `key`, word and `item`, a counter instance which contains pairs of (docID:frequency of occurence).

## Control flow

The `indexer` is implemented via file `indexer.c`. In addition to that, there is a module `indextest.c` to unit test all functions of `index` struct.

## Detailed function prototypes and their parameters
### main
This function parses arguments and initializes other modules.

`int main(const int argc, char* argv[])`
```
parse the command line, validate parameters, initialize other modules
call indexBuild, with pageDirectory
```

### indexBuild
This function builds an in-memory index from webpage files it finds in the pageDirectory.

`index_t* indexBuild(index_t* idx, const char* pageDirectory)`
```
creates a new 'index' object
  loops over document ID numbers, counting from 1
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage
```

### indexPage
This function scans a webpage document to add its words to the index.

`void indexPage(index_t* idx, webpage_t* page, int docID)`
```
steps through each word of the webpage,
   skips trivial words (less than length 3),
   normalizes the word (converts to lower case),
   looks up the word in the index,
     adding the word to the index if needed
   increments the count of occurrences of this word in this docID
```

## Other modules

### index
index module to the common library – a module to implement an abstract index_t type that represents an index in memory, and supports functions like index_new(), index_delete(), index_save(), and so forth.

#### index_new()
`index_t* index_new(const int num_slots)`

Creates a new index type (of size num_slots) and returns it, if unable to make, returns NULL.

#### index_delete()
`void index_delete(index_t* idx)`

Delete index struct and free all allocated memory by deleting hashtable and counters via their respective methods.

#### index_load()
`index_t* index_load(index_t* idx, const char* fileName)`

Loads the index struct from readable file.

#### index_save()
`void index_save(index_t* idx, const char* fileName)`

Writes the index struct in readable form in a file.

#### index_get()
`hashtable_t* index_get(index_t* idx)`

gets hashtable in the given index struct

### word
Module word.c (common between the Indexer and Querier) that implements normalizeWord (make words scanned from webpage lowercase).
#### wordNormalize()
`char* word_normalizeWord(char* word)`

 Normalizes and outputs the word for indexing. (Here, "normalize" means to convert all letters to lower-case.)

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status. We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

## Testing Implementation

### Integration testing

The `indexer`, as a complete program, was tested by building an index from pageDirectories produced by crawler (`toscrape` at depth 1 and `wikipedia` at depoth 1), and then the resulting index will be validated by running it through the `indextest` to ensure it can be loaded.

#### Test types
* with various invalid arguments
* no arguments
* one argument
* three or more arguments
* invalid pageDirectory (non-existent path)
* invalid pageDirectory (not a crawler directory)
* invalid indexFile (non-existent path)
* invalid indexFile (read-only directory)
* invalid indexFile (existing, read-only file)
* Valid Test for indexer (Create index file for crawler's toscrape depth 1) with valgrind
* Valid test for indextest (Validate indexer output) with valgrind
* Valid test for indexcmp (Check if created index is correct)
* Valid test for indextest (Check if created indextest.c produced same outputFile as inputfile)
* `echo$?` should be 0
* Valid Test for indexer (Create index file for crawler's toscrape depth 1) with valgrind
* Valid test for indextest (Validate indexer output) with valgrind
* Valid test for indextest (Check if created indextest.c produced same outputFile as inputfile)