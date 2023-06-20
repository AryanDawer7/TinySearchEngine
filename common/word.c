/**
 * word.c - CS50 word module
 * 
 * Add a module word.c (common between the Indexer and Querier) 
 * that implements normalizeWord (make words scanned from webpage
 * lowercase).
 * 
 * Aryan Dawer, April 2023
 */

#include <stdlib.h>
#include <ctype.h>
#include "word.h"

/************* word_normalizeWord() *************/
// see word.h for documentation
char* 
word_normalizeWord(char* word)
{
    if (word != NULL) {
        for (char* p = word; *p != '\0'; p++) {
            *p = tolower(*p);
        }
    }
    return word;
}