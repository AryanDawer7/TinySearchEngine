/*
 * crawler.h - header file for crawler module
 *
 * This specific crawler module is a program designed to 
 * store web pages from the Internet, implemented in one file called crawler.c. 
 * The program uses two data structures, a 'bag' of pages that need to be crawled and 
 * a 'hashtable' of URLs that have been seen during the crawl. 
 *
 * Usage: ./crawler seedURL pageDirectory maxDepth
 * 
 * Aryan Dawer, 2023
 */

#ifndef __CRAWLER_H
#define __CRAWLER_H

#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"

/**************** functions ****************/

/* 
 * The main function calls two other functions, parseArgs and crawl, to parse the 
 * command-line arguments and perform the crawling operation, respectively.
 */
int main(const int argc, char* argv[]);

/* 
 * The parseArgs function extracts the arguments from the command line, 
 * validates them, and returns if successful. For the seed URL, it normalizes and 
 * validates that it is an internal URL. For the page directory, it calls pagedir_init(). 
 * For the maximum depth, it ensures it is an integer within the specified range. 
 * If any issues are found, it prints an error to stderr and exits non-zero.
*/
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);

/* 
 * The crawl function is responsible for crawling from the seed URL to the maximum depth 
 * and saving pages in the page directory. It uses a hashtable and a bag data structure 
 * to keep track of URLs and webpages. It fetches HTML, saves webpages, scans for links, 
 * and deletes processed webpages.
*/
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);

/*
 * The pageScan function implements the pagescanner and extracts any links (URLs) from a given webpage. 
 * It ignores non-internal URLs and adds any new URLs found to the hashtable and bag. 
 * If a new URL is successfully added to the hashtable, a new webpage_t is created and added to the bag.
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

#endif // __CRAWLER_H