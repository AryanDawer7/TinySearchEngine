
/*
 * pagedir.h - header file for pagedir module
 *
 * re-usable module pagedir.c to handles the pagesaver mentioned in the design 
 * (writing a page to the pageDirectory), and marking it as a Crawler-produced 
 * pageDirectory (as required in the spec).
 *
 * Aryan Dawer, 2023
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

/**************** functions ****************/

/* Initializes and validates a pageDirectory to store webpage info by
 * creating a .crawler file in it.
 */
bool pagedir_init(const char* pageDirectory);

/* This function checks if the given directory is created by pagedir
 * by checking for a .crawler file in it
 */
bool pagedir_validate(const char* pageDirectory);

/* This function gets a pageDirectory and a docID and loads a webpage
 * with the information in that file.
*/
webpage_t* pagedir_load(const char* pageDirectory, const int docID);

/* This function saves the HTML of a webpage to a file, 
 * with the following format: URL, depth (as an integer), and the unmodified HTML.
 * It requires the webpage's structure, the path of the directory where the file 
 * will be saved, and an integer id number to name the file.
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

#endif // __PAGEDIR_H