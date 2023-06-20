/**
 * index.c - CS50 index module
 * 
 * index module to the common library – a module to implement an 
 * abstract index_t type that represents an index in memory, and 
 * supports functions like index_new(), index_delete(), index_save(), 
 * and so forth. 
 * 
 * Aryan Dawer, April 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "pagedir.h"
#include "index.h"
#include "word.h"

/**************** global types ****************/
typedef struct index {
    hashtable_t *ht;
    int num_slots;
} index_t;

/*********** local functions ***********/

void deleteHelper(void* item)
{
    counters_delete((counters_t*) item); //cast to counter and delete
}

void saveHelperByDocID(void* arg, const int key, const int item) 
{
    FILE* fp = mem_assert(arg, "Could not open/create file in write mode\n");
    if (key >= 0 && item > 0) {
        fprintf(fp, "%d %d ", key, item);
    }
}

void saveHelper(void* arg, const char* key, void* item)
{
    FILE* fp = mem_assert(arg, "Could not open/create file in write mode\n");
    counters_t* counter = mem_assert(item, "Counter is NULL"); // cast void to counter
    if (key != NULL) {
        fprintf(fp, "%s ", key); // write the word on line
        counters_iterate(counter, fp, saveHelperByDocID); // iterate id and count for different files
        fprintf(fp, "\n"); // newline
    }
}

/**************** functions ****************/

/**************** index_new() ****************/
index_t* 
index_new(const int num_slots)
{
    index_t* index = mem_malloc_assert(sizeof(index_t), "Error allocating memory\n");
    index->ht = mem_assert(hashtable_new(num_slots), "Error creating hashtable\n");
    index->num_slots = num_slots;
    return index;
}

/**************** index_delete() ****************/
void 
index_delete(index_t* idx)
{
    if (idx == NULL) {
        fprintf(stderr, "NULL argument entered\n");
        exit(1);
    }
    hashtable_delete(idx->ht, deleteHelper);
    mem_free(idx);
}

/**************** index_save() ****************/
void 
index_save(index_t* idx, const char* fileName)
{
    if (idx == NULL || fileName == NULL) {
        fprintf(stderr, "NULL argument entered\n");
        exit(1);
    }
    FILE* fp = mem_assert(fopen(fileName, "w"), "Couldn't open file in write mode!\n");
    hashtable_iterate(idx->ht, fp, saveHelper); // write in file
    fclose(fp); // close file
}

/**************** index_load() ****************/
index_t*
index_load(index_t* idx, const char* fileName)
{
    if (idx == NULL || fileName == NULL) {
        fprintf(stderr, "NULL argument entered\n");
        exit(1);
    }
    FILE* fp = mem_assert(fopen(fileName, "r"), "Couldn't open file in read mode!\n");
    char* word;
    int docID;
    int count;
    counters_t* counter;
    while ((word = file_readWord(fp)) != NULL) {
        counter = hashtable_find(idx->ht, word_normalizeWord(word));
        while (fscanf(fp, "%d %d ", &docID, &count) > 0) {
            if (counter != NULL) {
                counters_set(counter, docID, count);
            } else {
                counter = mem_assert(counters_new(), "Error allocating memory\n");
                counters_set(counter, docID, count);
                hashtable_insert(idx->ht, word_normalizeWord(word), counter);
            }
        }
        mem_free(word);
    }
    mem_free(word);
    fclose(fp);
    return idx;
}

/**************** index_get() ****************/
hashtable_t* index_get(index_t* idx)
{
	return idx->ht;
}