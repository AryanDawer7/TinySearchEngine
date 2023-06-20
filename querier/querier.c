/**
 * querier.c - CS50 querier module
 * 
 * The TSE Querier is a standalone program that reads the index 
 * file produced by the TSE Indexer, and page files produced by 
 * the TSE Querier, and answers search queries submitted via 
 * stdin.
 * 
 * Aryan Dawer, May 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../libcs50/mem.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../common/pagedir.h"


/******************** local types *******************/
typedef struct twoCntrs {
    counters_t* final;
    counters_t* toAdd;
} twoCntrs_t;

typedef struct doc {
    counters_t* docScores;
    int* highest;
} doc_t;

/*************** Function Prototypes ****************/
/* The fileno() function is provided by stdio,
 * but for some reason not declared by stdio.h, so declare here.
 */
int fileno(FILE *stream);

/* Prints interactive prompt if stdin is taken from keyboard instead of file
*/
static void prompt(void);

/* Checks query for any special characters of empty string,
 * if found, exits non-zero with relevant stderr
 * if not, returns number of tokens
*/
static bool checkQuery(char* query);

/* Counts and returns the number of tokens in query
 * Assumption: Query does not contain special characters
*/
static int countNumTokens(char* query);

/* Creates array of tokens
*/
static void makeTokenArray(char* query, int numTokens, char** tokens);

/* Checks for any invalid and/or format
*/
static bool checkAndOr(int numTokens, char** tokens);

/* Goes through all tokens/ logical operators scores documents
 * based on interpreted logic.
*/
static counters_t* computeQuery(index_t* idx, char** tokens, int numTokens);

/* interscets two counters with matching documents by choosing lowest value
*/
static counters_t* countersIntersect(counters_t* initial, counters_t* new);

/* unions two counters with matching documents by setting score to sum of their values
*/
static counters_t* countersUnion(counters_t* initial, counters_t* new);

/* iterates over one counter and gives its value to the  other counter
*/
static void duplicateCounters(void *arg, int key, int count);

/* helps to find lowest val for counters intersect
*/
static void countersIntersectHelp(void *arg, int key, int count);

/* helps to find sum of values for counters union
*/
static void countersUnionHelp(void *arg, int key, int count);

/* ranks and prints documents given docScores counter
*/
static void rankAndPrintDocs(char* pageDirectory, counters_t* docScores);

/* counts how many docs there are in docScores
*/
static void countTotalDocsHelp(void *arg, int key, int count);

/* helps doc ranking by setting highestDoc to the doc with 
 * highest score by iterating over docscores
*/
static void rankDocHelp(void *arg, int key, int count);

int
main(const int argc, char* argv[])
{
    // Parse the command line, validate parameters
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments!\nUsage: ./querier pageDirectory indexFilename\n");
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

    // Check if file is readable
    char* fileName = mem_assert(argv[2], "Couldn't read terminal input!\n");
    FILE* fp = fopen(fileName, "r");
    int numLines;
	if (fp == NULL) {
		fprintf(stderr, "Couldn't open %s in read mode.\nMake sure given path exists and all directories and file allow write mode\n", fileName);
		exit(3);
	} else {
        numLines = file_numLines(fp);
		fclose(fp);
	}

    // Create and load into index
    index_t* idx = index_new(numLines);
    index_load(idx, fileName);

    char* query;
    prompt(); // interactive 
    while ((query = file_readLine(stdin)) != NULL) {
        if (!checkQuery(query)) { // check for bad characters
            mem_free(query);
            prompt();
            continue;
        }
        int numTokens = countNumTokens(query); // count number of tokens in query

        char* tokens[numTokens];
        mem_assert(tokens, "Error allocating memory");
        makeTokenArray(query, numTokens, tokens); // split query into token list
        
        if (!checkAndOr(numTokens, tokens)) { // check for and/or errors
            mem_free(query);
            prompt();
            continue;
        }

        // Print clean query
        printf("Query: ");
        for (int i = 0; i < numTokens; i++) {
            printf("%s", tokens[i]);
            if (i != numTokens-1) {
                printf(" ");
            } else {
                printf("\n");
            }
        }

        // The main document scoring starts here
        counters_t* docScores = computeQuery(idx, tokens, numTokens);

        rankAndPrintDocs(pageDirectory, docScores);

        mem_free(query);
        prompt();
        
    }

    index_delete(idx); // free idx allocated memory

    return 0; // Exit zero at EOF
}

static void
prompt(void)
{
    // print a prompt if stdin is a tty (terminal)
    if (isatty(fileno(stdin))) {
        printf("Query? ");
    }
}

static bool 
checkQuery(char* query)
{
    char* character = query;
    int numSpaces = 0;
    while (*character != '\0') {
        if (!isspace(*character) && !isalpha(*character)) {
            fprintf(stderr, "Error: bad character '%c' in query.\n", *character);
            return false;
        } else if (isspace(*character)) {
            numSpaces++;
        }
        character++;
    }
    if (numSpaces == strlen(query)) {
        fprintf(stderr, "Error: query doesn't contain any tokens.\n");
        return false;
    }
    return true;
}

static int 
countNumTokens(char* query)
{
    char* token = query;
    int num = 0;
    while (*token != '\0') {
        while (!isalpha(*token)) {
            if (*token == '\0') {
                break;
            }
            token++;
        }
        if (isalpha(*token)) {
            num++;
        }
        while ((isalpha(*token)) && (*token != '\0')) {
            token++;
        }
    }
    return num;
}

static void 
makeTokenArray(char* query, int numTokens, char** tokens)
{
    char* token = query;
    char* tokenEnd = NULL;
    for (int i = 0; i < numTokens; i++) {
        while (!isalpha(*token)) {
            token++;
        }
        if (isalpha(*token)) {
            tokenEnd = token;
        }
        while (isalpha(*tokenEnd)) {
            tokenEnd++;
        }

        *tokenEnd = '\0';
        
        tokens[i] = word_normalizeWord(token);
        
        tokenEnd++;
        token = tokenEnd;
    }
}

static bool
checkAndOr(int numTokens, char** tokens)
{
    if (strcmp(tokens[0], "and") == 0) {
        fprintf(stderr, "Error: 'and' cannot be first\n");
        return false;
    }
    if (strcmp(tokens[0], "or") == 0) {
        fprintf(stderr, "Error: 'or' cannot be first\n");
        return false;
    }
    if (strcmp(tokens[numTokens-1], "and") == 0) {
        fprintf(stderr, "Error: 'and' cannot be last\n");
        return false;
    }
    if (strcmp(tokens[numTokens-1], "or") == 0) {
        fprintf(stderr, "Error: 'or' cannot be last\n");
        return false;
    }
    for (int i=1;i<numTokens-1;i++) {
        if (strcmp(tokens[i], "and") == 0) {
            if (strcmp(tokens[i+1], "or") == 0) {
                fprintf(stderr, "Error: 'and' and 'or' cannot be adjacent\n");
                return false;
            }
            if (strcmp(tokens[i+1], "and") == 0) {
                fprintf(stderr, "Error: 'and' and 'and' cannot be adjacent\n");
                return false;
            }
        }
        if (strcmp(tokens[i], "or") == 0) {
            if (strcmp(tokens[i+1], "or") == 0) {
                fprintf(stderr, "Error: 'or' and 'or' cannot be adjacent\n");
                return false;
            }
            if (strcmp(tokens[i+1], "and") == 0) {
                fprintf(stderr, "Error: 'or' and 'and' cannot be adjacent\n");
                return false;
            }
        }
    }
    return true;
}

static counters_t* 
computeQuery(index_t* idx, char** tokens, int numTokens)
{
    counters_t* queryRunCount = mem_assert(counters_new(), "Error allocating memory to counter!\n");
    counters_t* andSeqRunCount;
    counters_t* curr = NULL;
    bool newAndSeq = true;


    for (int i = 0; i < numTokens; i++) {
        if (strcmp(tokens[i], "and") == 0) { // All "and" are a part of the same andSequence
            continue;
        }
        if (strcmp(tokens[i], "or") == 0) {
            counters_t* updatedQueryRunCount = countersUnion(queryRunCount, andSeqRunCount);

            counters_delete(queryRunCount);
            counters_delete(andSeqRunCount);

            newAndSeq = true;
            queryRunCount = updatedQueryRunCount;
            continue;
        }
        curr = hashtable_find(index_get(idx), tokens[i]);
        if (curr == NULL) {
            newAndSeq = true;
            if (i == numTokens - 1){
                return queryRunCount;
            }
            i++;
            while (strcmp(tokens[i], "or") != 0 && i != numTokens - 1) {
                i++;
            }
            if (strcmp(tokens[i], "or") == 0) {
                continue;
            } else if (i == numTokens - 1){
                return queryRunCount;
            }
        } else {
            if (newAndSeq) {
                andSeqRunCount = mem_assert(counters_new(), "Error allocating memory to counter!\n");
                counters_iterate(curr, andSeqRunCount, duplicateCounters);
                newAndSeq = false;
            } else {
                counters_t* updatedAndSeqRunCount = countersIntersect(andSeqRunCount, curr);
                andSeqRunCount = updatedAndSeqRunCount;
            }
        }
    }
    // for the last andSequence
    if (andSeqRunCount != NULL) {
        counters_t* updatedQueryRunCount = countersUnion(queryRunCount, andSeqRunCount);
        counters_delete(queryRunCount);
        counters_delete(andSeqRunCount);
        queryRunCount = updatedQueryRunCount;
    }
    return queryRunCount;
}

static void 
duplicateCounters(void *arg, int key, int count)
{
    counters_t* output = mem_assert(arg, "NULL arg");
    counters_set(output, key, count);
}

static counters_t*
countersIntersect(counters_t* initial, counters_t* new)
{
    counters_t* output = mem_assert(counters_new(), "Error allocating memory to counter!\n");

    mem_assert(initial, "NULL counter for counter Intersect!\n");
    mem_assert(new, "NULL counter for counter Intersect!\n");

    counters_iterate(initial, output, duplicateCounters);

    twoCntrs_t* twoCounters = mem_malloc_assert(sizeof(twoCntrs_t), "Error allocating memory to twoCntrs!\n");
    twoCounters->final = output;
    twoCounters->toAdd = new;

    counters_iterate(output, twoCounters, countersIntersectHelp);

    return output;
}

static void
countersIntersectHelp(void *arg, int key, int count) {
    twoCntrs_t* twoCounters = mem_assert(arg, "NULL arg");
    int lowest = count;
    if (count > counters_get(twoCounters->toAdd, key)) {
        lowest = counters_get(twoCounters->toAdd, key);
    }
    counters_set(twoCounters->final, key, lowest);
}

static counters_t* 
countersUnion(counters_t* initial, counters_t* new)
{
    counters_t* output = mem_assert(counters_new(), "Error allocating memory to counter!\n");

    mem_assert(initial, "NULL counter for counter Intersect!\n");
    mem_assert(new, "NULL counter for counter Intersect!\n");

    counters_iterate(initial, output, duplicateCounters);
    counters_iterate(new, output, countersUnionHelp);

    return output;
}

static void
countersUnionHelp(void *arg, int key, int count) {
    counters_t* updatedOutput = mem_assert(arg, "NULL arg");
    counters_set(updatedOutput, key, counters_get(updatedOutput, key) + count);
}

static void 
rankAndPrintDocs(char* pageDirectory, counters_t* docScores)
{
    int count = 0;
    counters_iterate(docScores, &count, countTotalDocsHelp);
    if (count == 0) {
        printf("No Matching documents found...\n");
        printf("-----------------------------------------------\n");
        return;
    }

    printf("Matches %d documents (ranked):\n", count);

    int highestDocScore = -1;
    int highestDoc = 0;
    doc_t* docScoreCheck = mem_malloc_assert(sizeof(doc_t), "Error allocating memory to doc_t!\n");
    docScoreCheck->docScores = docScores;
    while (highestDocScore != 0) {
        docScoreCheck->highest = &highestDoc;
        counters_iterate(docScores, docScoreCheck, rankDocHelp);
        highestDocScore = counters_get(docScores, highestDoc);
        if (highestDocScore != 0) {
            webpage_t* page = mem_assert(pagedir_load(pageDirectory, highestDoc), "Couldn't open page!\n");
            printf("score %4d doc %4d: %s\n", highestDocScore, highestDoc, webpage_getURL(page));
            webpage_delete(page);
            counters_set(docScores, highestDoc, 0);
        }
    }
    printf("-----------------------------------------------\n");
}

static void
countTotalDocsHelp(void *arg, int key, int count) {
    int* output = mem_assert(arg, "NULL arg");
    if (count != 0) {
        *output = *output + 1;
    }
}

static void
rankDocHelp(void *arg, int key, int count) {
    int* highest = mem_assert(((doc_t*)arg)->highest, "NULL arg");
    counters_t* docScores = mem_assert(((doc_t*)arg)->docScores, "NULL arg");

    if (*highest == -1 || counters_get(docScores, *highest) < count) {
        *highest = key;
    }
}