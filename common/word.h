/**
 * word.h - header file for word module
 * 
 * Add a module word.c (common between the Indexer and Querier) 
 * that implements normalizeWord (make words scanned from webpage
 * lowercase).
 * 
 * Aryan Dawer, April 2023
 */

#ifndef __WORD_H
#define __WORD_H

#include <string.h>
#include <ctype.h>

/************* word_normalizeWord() *************/
/** 
 * Normalizes" and outputs the word before indexing. 
 * (Here, "normalize" means to convert all letters to lower-case.)
 */

char* word_normalizeWord(char* word);

#endif // __WORD_H