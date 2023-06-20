/* 
* pagedir.c - CS50 module for TSE
* re-usable module pagedir.c to handles the pagesaver mentioned in the design 
* (writing a page to the pageDirectory), and marking it as a Crawler-produced 
* pageDirectory (as required in the spec).
* 
* see pagedir.h for more information.
*
* Aryan Dawer, April 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

/**************** pagedir_init() ****************/
bool 
pagedir_init(const char* pageDirectory)
{
    // Make pointer to filePath string
    char* filePath = mem_malloc_assert(sizeof(char) * (strlen(pageDirectory) + 10), "Error allocating memory!");
    strcpy(filePath, pageDirectory);
    strcat(filePath, "/.crawler");

    // Create file and check if it was created
    FILE* fp = fopen(filePath, "w");
    if (fp != NULL) {
        mem_free(filePath);
        fclose(fp);
        return true;
    } else {
        mem_free(filePath);
        return false;
    }
}

/**************** pagedir_validate() ****************/
bool
pagedir_validate(const char* pageDirectory)
{
    // Make pointer to filePath string
    char* filePath = mem_malloc_assert(sizeof(char) * (strlen(pageDirectory) + 10), "Error allocating memory!");
    strcpy(filePath, pageDirectory);
    strcat(filePath, "/.crawler");

    // Check if file exists
    FILE* fp = fopen(filePath, "r");
	if (fp == NULL) {
        mem_free(filePath);
		return false;
	}
    fclose(fp);
    mem_free(filePath);
    return true;
}

/**************** pagedir_load() ****************/
webpage_t*
pagedir_load(const char* pageDirectory, const int docID)
{
    // Make pointer to filePath string
    char* strDocID = mem_malloc_assert(sizeof(int), "Error allocating memory!");
    sprintf(strDocID, "%d", docID);
    char* filePath = mem_malloc_assert(sizeof(char) * (strlen(pageDirectory) + strlen(strDocID) + 2), "Error allocating memory!");
                                       // len(pageDirectory) + len("/") + len(docID) + len("\0")
    strcpy(filePath, pageDirectory);
    strcat(filePath, "/");
    strcat(filePath, strDocID);
    
    // Create and open file for writing
    
    FILE* fp = fopen(filePath, "r");

    mem_free(strDocID); // no use for docID except for string except for concatination
    mem_free(filePath); // must free this before use of mem_assert to ensure no leaks

    webpage_t* page = NULL;

    if (fp != NULL) {
        // get info to make webpage struct
        char* url = file_readLine(fp);
        char* depth = file_readLine(fp);
        char* html = file_readFile(fp);
        int depthInt = atoi(depth);
        mem_free(depth);
        
        page = webpage_new(url, depthInt, html);
        fclose(fp);
    }

    return page;
}

/**************** pagedir_save() ****************/
void 
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
    // Make pointer to filePath string
    char* strDocID = mem_malloc_assert(sizeof(int), "Error allocating memory!");
    sprintf(strDocID, "%d", docID);
    char* filePath = mem_malloc_assert(sizeof(char) * (strlen(pageDirectory) + strlen(strDocID) + 2), "Error allocating memory!");
                                       // len(pageDirectory) + len("/") + len(docID) + len("\0")
    strcpy(filePath, pageDirectory);
    strcat(filePath, "/");
    strcat(filePath, strDocID);
    
    // Create and open file for writing
    FILE* fp = fopen(filePath, "w");

    mem_free(strDocID); // no use for docID except for string except for concatination
    mem_free(filePath); // must free this before use of mem_assert to ensure no leaks

    mem_assert(fp, "Error opening file\n");

    // Get URl, depth and html content and write it in the file
    fprintf(fp, "%s\n", webpage_getURL(page));      // Line 1
    fprintf(fp, "%d\n", webpage_getDepth(page));    // Line 2
    fprintf(fp, "%s", webpage_getHTML(page));       // Line 3
    fclose(fp);
}