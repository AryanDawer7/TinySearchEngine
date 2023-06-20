# CS50 TSE Querier

## Querier Pseudocode
```
parse the command line, validate parameters
check if pageDirectory created by crawler
check if file is readable
create and load into index
start loop of reading queries
    check for bad characters
    count number of tokens in query
    split query into token list
    check for and/or errors
    print clean query
    The main document scoring starts here
        score documents
        rank documents using scoreCounts
        print documents
        free memory allocs
exit zero
```

## Datatype Description

The querier employs different data structures to arrange information, mainly `counters` and `index` module. 


The `index` structure is utilized to load and store  the andsequences of all words in the query and contains (word, counters) pairings. Each counters struct within this index comprises file IDs coupled with their corresponding score. Comparing all counters within the index allows for determining their sums.


The `twoCntrs` structure is advantageous when seeking intersections and unions by permitting the passing of two counters as one argument. It encompasses two pointers to counters structures. 


The `doc` structure bundles the docScores counter and the docID with the highest score.