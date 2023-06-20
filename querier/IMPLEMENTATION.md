# CS50 TSE Querier

## Specific Implementation Details
The pseudocode for the querier can be found in the design spec 
and the following are all the methods and their descriptions 
which were used to implement each line of the pseudocode.
### Function Prototypes and Description
```c
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
```

## Testing Implementation

### Testing with Invalid queries

The following queries all have syntax errors:

```
and
or
and earth
or earth
planet earth or
planet earth and
planet earth and or science
planet earth and and science
planet earth or and science
Warning!
(The Lunar Chronicles #4.5)
```

because (per the syntax)

 * the literal 'and' must come between `words`, and the literal 'or' must come between `andsequences`, so they cannot appear at the beginning or end of a query,
 * the literals ('and' & 'or') cannot be adjacent
 * characters other than letters or spaces are disallowed.

### Testing with Fuzz queries

The given program `fuzzquery.c`, creates a number of random queries from words and logical operators found in the given index.

The following queries were generated using it on `toscrape-1` with `numQueries = 10` and `seed = 11111`:

```
walking AND alarms invisible hannibal
publication AND victorious hollow AND france
mouths AND rage vol allowed OR wears seriously
evolution
angels OR race shrubby AND ago
une
trip
parricides activist
above OR promise
grand AND bands
```