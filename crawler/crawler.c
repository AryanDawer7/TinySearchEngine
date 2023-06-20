/* 
* crawler.c - CS50 module for TSE
* This specific crawler module is a program designed to 
* store web pages from the Internet, implemented in one file called crawler.c. 
* The program uses two data structures, a 'bag' of pages that need to be crawled and 
* a 'hashtable' of URLs that have been seen during the crawl. 
*
* Usage: ./crawler seedURL pageDirectory maxDepth
* 
* see crawler.h for more information.
*
* Aryan Dawer, April 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <ctype.h>
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"

#define HASHTABLE_SLOTS 200

/**************** function prototypes ****************/
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/**************** main() ****************/
int
main(const int argc, char* argv[])
{
    // If invalid number of args, exit non-zero & print usage
    if (argc != 4) {
        fprintf(stderr, "Invalid number of arguments. Usage: ./crawler seedURL pageDirectory maxDepth\n");
        return 1;
    }

    char* seedURL = mem_malloc_assert(strlen(argv[1]) + 1, "Error allocating memory!");
    char* pageDirectory = mem_malloc_assert(strlen(argv[2]) + 1, "Error allocating memory!");
    int maxDepth;

    // Call command-line argument parser
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);

    // Crawl the web
    crawl(seedURL, pageDirectory, maxDepth);

    // Cleanup and exit zero
    mem_free(pageDirectory);
    return 0;
}

/**************** parseArgs() ****************/
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth)
{
    // Parse and validate seedURL
    char* normURL = normalizeURL(argv[1]);
    if (normURL == NULL) {            // I have included this because normalize doesn't work with 
                                     // with urls like HTTP://www.google.com
            fprintf(stderr, "%s could not be normalized, it is either not internal or invalid!\n", argv[1]);
            mem_free(normURL);
            mem_free(*seedURL); mem_free(*pageDirectory);
            exit(2);
    } else {
        strcpy(*seedURL, normURL);
    }
    mem_free(normURL);


    if (*seedURL != NULL) {
        if (!isInternalURL(*seedURL)) {
            fprintf(stderr, "%s is not an internal URL!\n", *seedURL);
            mem_free(*seedURL); mem_free(*pageDirectory);
            exit(2);
        }
    } else {
        fprintf(stderr, "Couldn't process URL as command-line argument!\n");
        mem_free(*seedURL); mem_free(*pageDirectory);
        exit(1);
    }

    // Parse and validate maxDepth
    for (int i=0;i<strlen(argv[3]);i++) {
        if (!isdigit(argv[3][i])) {
            fprintf(stderr, "maxDepth must be an integer!\n");
            mem_free(*seedURL); mem_free(*pageDirectory);
            exit(3);
        }
    }
    *maxDepth = atoi(argv[3]);
    if (maxDepth != NULL) {
        if (*maxDepth < 0 || *maxDepth > 10) {
            fprintf(stderr, "maxDepth must be an integer in the range [1,10]!\n");
            mem_free(*seedURL); mem_free(*pageDirectory);
            exit(4);
        }
    } else {
        fprintf(stderr, "Couldn't process maxDepth as command-line argument!\n");
        mem_free(*seedURL); mem_free(*pageDirectory);
        exit(1);
    }

    // Parse, initialize and validate pageDirectory
    strcpy(*pageDirectory, argv[2]);
    
    // Check if dir exists
    DIR* pd = opendir(*pageDirectory);
	if (pd == NULL) {
		fprintf(stderr, "Input pageDirectory couldn't be opened\n");
        mem_free(*seedURL); mem_free(*pageDirectory);
		exit(5);
	} else {
        closedir(pd);
    }

    if (*pageDirectory != NULL) {
        if (!pagedir_init(*pageDirectory)) {
            fprintf(stderr, "Failed to initialize page directory %s\n", *pageDirectory);
            mem_free(*seedURL); mem_free(*pageDirectory);
            exit(5);
        }
    } else {
        fprintf(stderr, "Couldn't process pageDirectory as command-line argument!\n");
        mem_free(*seedURL); mem_free(*pageDirectory);
        exit(1);
    }

}

/**************** crawl() ****************/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
    int id = 1;         // Initialise doucmentID for filename
    webpage_t* page = NULL;   // Initialise webpage object for loop

    // Initialise hashtable
    hashtable_t* pagesSeen = hashtable_new(HASHTABLE_SLOTS);
    if (pagesSeen == NULL) {
        fprintf(stderr, "Failed to create hashtable.\n");
        exit(6);
    }

    // Initialise bag
    bag_t* pagesToCrawl = bag_new();
    if (pagesToCrawl == NULL) {
        fprintf(stderr, "Failed to create bag.\n");
        exit(7);
    }

    // Add seed URL to hashtable and bag
    webpage_t* seedPage = webpage_new(seedURL, 0, NULL);
    if (seedPage == NULL) {
        fprintf(stderr, "Failed to create seed page.\n");
        exit(8);
    }
    
    // printf("Depth: %d    Found: %s\n", 0, seedURL);
    // printf("Depth: %d    Added: %s\n", 0, seedURL);
    hashtable_insert(pagesSeen, seedURL, "");
    bag_insert(pagesToCrawl, seedPage);

    // while bag is not empty
    while ((page = bag_extract(pagesToCrawl)) != NULL) {
        // fetch the HTML for that webpage
        if(webpage_fetch(page)) {
            // save the webpage to pageDirectory
            pagedir_save(page, pageDirectory, id);
            id++;
            // if the webpage is not at maxDepth,
            if (webpage_getDepth(page) < maxDepth) {
                // pageScan that HTML
                pageScan(page, pagesToCrawl, pagesSeen);
            }
        } else {
            fprintf(stderr, "Failed to get webpage info.\n");
        }
        // Delete that webpage
        webpage_delete(page);
    }
    // delete the hashtable
    hashtable_delete(pagesSeen, NULL);
    // delete the bag — even though bag is empty, webpage_delete for good measure
    bag_delete(pagesToCrawl, webpage_delete);
}

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    if (page == NULL) {
        fprintf(stderr, "Invalid webpage!\n");
        exit(9);
    }

    // Extract any links (URLs) from the HTML
    int pos = 0;
    char* url;
    // while there is another URL in the page
    while ((url = webpage_getNextURL(page, &pos)) != NULL) {
        // if that URL is Internal
        // printf("Depth: %d    Found: %s\n", webpage_getDepth(page) + 1, url);
        if (isInternalURL(url)) {
            // insert the webpage into the hashtable
            if (hashtable_insert(pagesSeen, url, "")) {
                // if that succeeded, create a webpage_t for it
                webpage_t* newPage = webpage_new(url, webpage_getDepth(page) + 1, NULL);
                // insert the webpage into the bag
                if (newPage != NULL) {
                    // printf("Depth: %d    Added: %s\n", webpage_getDepth(newPage), url);
                    bag_insert(pagesToCrawl, newPage);
                }
            } else {
                // printf("Depth: %d    Ignored: %s\n", webpage_getDepth(page) + 1, url);
                mem_free(url);
            }
        } else {
            // printf("Depth: %d    Ignored: %s\n", webpage_getDepth(page) + 1, url);
            mem_free(url);
        }
    }
}