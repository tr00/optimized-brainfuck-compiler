/**
 *      PATTERN MATCHING OPTIMIZER
 * 
 * it works by shifting over the input and then reducing
 * a matching pattern to a semantically equivalent but
 * more concise substitution.
 *  
 * ... SKIP, DECR, LOOP, ... => ... SETV, NOOP, NOOP, ...
 * 
 * (every pattern starts with any amount of NOOP's)
 * 
 * The patterns are stored in a trie with 
 * O(m) lookup where m is the length of the pattern.
 * 
 * The substitutions are stored in an array
 * which is indexed by each (accepting) pattern
 * so the lookup for substitutions is O(1).
 * 
 * The subst-array may contain duplicate
 * substitutions for different patterns.
 * this could be avoided but that would slow down insertion.
 * 
 */
#include "all.h"

#define _NODE_COUNT 9


// I will move a lot of this into the header once this is done
typedef struct 
{
    struct NODE
    {
        struct NODE *next[_NODE_COUNT];
        char *subst;
    } root;
    char **substitutions;
} PatternMatcher;

void init(PatternMatcher *handle)
{
    handle->root.next[NOOP] = &handle->root;
    for(size_t i = 1; i < _NODE_COUNT; i++)
        handle->root.next[i] = NULL;
}

void shift(PatternMatcher *handle, char instr);

void reduce(PatternMatcher *handle);

void cleanup(PatternMatcher *handle);
