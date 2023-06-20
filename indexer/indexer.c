/**
 * indexer.c - CS50 indexer module
 * 
 * TSE indexer is a standalone program that reads the document files
 * produced by the TSE crawler, builds an index, and writes that index to a file. 
 * Its companion, the index tester, loads an index file produced by the indexer 
 * and saves it to another file.
 * 
 * Aryan Dawer, April 2023
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
#include "dirent.h"

/*************** Function Prototypes ****************/
/*
 * This method builds the index struct from
 * the files within pageDirectory. 
 */
index_t* indexBuild(index_t* idx, const char* pageDirectory);

/*
 * This method helps build index struct one webpage at a time
 */
void indexPage(index_t* idx, webpage_t* page, int docID);

/*************** functions ****************/

int
main(const int argc, char* argv[])
{
    // Parse the command line, validate parameters
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments!\nUsage: ./indexer pageDirectory indexFilename\n");
        exit(1);
    }

    // Check if pageDirectory created by crawler
    char* pageDirectory = mem_assert(argv[1], "Couldn't read terminal input!\n");
    DIR* pd = opendir(pageDirectory);
	if (pd == NULL) {
		fprintf(stderr, "Input pageDirectory doesn't exist\n");
		exit(2);
	} else {
        closedir(pd);
    }

    if (!pagedir_validate(pageDirectory)) {
        fprintf(stderr, "Given directory not created by crawler!\n");
        exit(2);
    }

    // Check if file is writable
    char* fileName = mem_assert(argv[2], "Couldn't read terminal input!\n");
    FILE *fp = fopen(fileName, "w");
	if (fp == NULL) {
		fprintf(stderr, "Couldn't open %s in write mode.\nMake sure given path exists and all directories and file allow write mode\n", fileName);
		exit(3);
	} else {
		fclose(fp);
	}

    // Initialize important modules
    index_t* idx = NULL;

    // build index
    idx = indexBuild(idx, pageDirectory);

    // save index
    index_save(idx, fileName);

    // delete index
    index_delete(idx);

    return 0;
}

index_t* indexBuild(index_t* idx, const char* pageDirectory)
{
    idx = index_new(600);
    
    int docID = 1;
    webpage_t* page;
    while ((page = pagedir_load(pageDirectory, docID)) != NULL) {
        indexPage(idx, page, docID);
        webpage_delete(page);
        docID++;
    }
    return idx;
}

void indexPage(index_t* idx, webpage_t* page, int docID)
{
    int pos = 0;
    char* word;
    counters_t* counter;
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(word)>=3) {
            word = word_normalizeWord(word);
            if ((counter = hashtable_find(index_get(idx), word)) != NULL) {
                counters_add(counter, docID);
            } else {
                counter = mem_assert(counters_new(), "Error allocating memory\n");
                counters_add(counter, docID);
                hashtable_insert(index_get(idx), word, counter);
            }
        }
        mem_free(word);
    }
    mem_free(word);
}