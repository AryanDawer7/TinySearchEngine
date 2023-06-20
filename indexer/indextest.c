/**
 * indextest.c - CS50 indextest module
 * 
 * The index tester shall execute from a command line with 
 * usage: ./indextest oldIndexFilename newIndexFilename,
 * load the index from the oldIndexFilename into an 
 * inverted-index data structure. create a file 
 * newIndexFilename and write the index to that file.
 * 
 * Aryan Dawer, April 2023
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"

int 
main(int argc, char *argv[])
{
    // Parse the command line, validate parameters
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments!\nUsage: ./indexer pageDirectory indexFilename\n");
        exit(1);
    }
    if (argv[1] == NULL || argv[2] == NULL) {
        fprintf(stderr, "NULL arguments entered!");
        exit(2);
    }

    // Create index and load it with info
    index_t* idx = index_new(600);
    index_load(idx, argv[1]);
    
    // write this info into newFile
    index_save(idx, argv[2]);
    index_delete(idx);

    return 0;
}