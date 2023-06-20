/**
 * index.h - header file for index module
 * 
 * index module to the common library – a module to implement an 
 * abstract index_t type that represents an index in memory, and 
 * supports functions like index_new(), index_delete(), index_save(), 
 * and so forth. 
 * 
 * Aryan Dawer, April 2023
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "pagedir.h"
#include "word.h"

/**************** global types ****************/
typedef struct index index_t;    // opaque to users of the module

/*********** local functions ***********/

/**************** deleteHelper() ****************/
/**
 * Helps delete the item of the hashtable which would be a 
 * counter struct in this case.
*/
void deleteHelper(void* item);

/**************** saveHelper() ****************/
/**
 * Helps write the item to the file by using the _iterator
 * methods in hashtable and counters. Specifically iterates over 
 * keys and calls countera_iterator.
*/
void saveHelper(void* arg, const char* key, void* item);

/**************** saveHelperByDocID() ****************/
/**
 * Helps write the item to the file by using the _iterator
 * methods in hashtable and counters. Specifically iterates over 
 * counters
*/
void saveHelperByDocID(void* arg, const int key, const int item);

/**************** functions ****************/

/**************** index_new() ****************/
/**
 * Creates a new index type (of size num_slots) and returns it, 
 * if unable to make, returns NULL.
*/
index_t* index_new(const int num_slots);

/**************** index_Delete() ****************/
/**
 * Deletes index, free allocated memory.
*/
void index_delete(index_t* idx);

/**************** index_save() ****************/
/**
 * Writes the index struct in readable form in a file.
*/
void index_save(index_t* idx, const char* fileName);

/**************** index_load() ****************/
/**
 * Loads the index struct from readable file.
*/
index_t* index_load(index_t* idx, const char* fileName);

/**************** index_get() ****************/
/**
 * gets hashtable in the given index struct
*/
hashtable_t* index_get(index_t* idx);

#endif // __INDEX_H