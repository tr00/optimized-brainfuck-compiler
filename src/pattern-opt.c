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
 * Usage:
 * 
 * void traverse(char *repr, size_t length)
 * {
 *      PatternMatcher handle;
 * 
 *      init(&handle);
 * 
 *      for(size_t i = 0; i < length; i++)
 *      {
 *          shift(&handle, repr[i]);
 * 
 *          if(end of peephole)
 *          {
 *              i -= reduce(&handle, repr);
 *          }
 *      }
 * 
 *      cleanup(&handle);
 * }
 * 
 * 
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "all.h"

#define _NODE_COUNT 9

// I will move a lot of this into the header once its done
typedef struct
{
    /**
     * Trie datastructure for pattern lookup
     */
    struct NODE
    {
        struct NODE *next[_NODE_COUNT];
        size_t index;
    } root;
    /**
     * Vector of substitutions
     */
    struct
    {
        size_t len, cap;
        uint8_t **substitutions;
    };
    /**
     * Variables used for keeping track of the current 
     * position inside the trie
     */
    struct
    {
        struct NODE *current;
        size_t depth;
    };
} PatternMatcher; // size: 15 * 8B

void init(PatternMatcher *handle)
{
    handle->root.next[NOOP] = &handle->root;
    for (size_t i = 1; i < _NODE_COUNT; i++)
        handle->root.next[i] = NULL;

    handle->len = 0;
    handle->cap = 40;
    handle->substitutions = safe_malloc(handle->cap * sizeof(uint8_t *));
    handle->current = &handle->root;
    handle->depth = 0;
    // TODO: add default patterns
}

static struct NODE *newnode()
{
    struct NODE *node = safe_malloc(sizeof(struct NODE));

    for (size_t i = 0; i < _NODE_COUNT; i++)
        node->next[i] = NULL;

    node->index = 0; // zero is treated as an invalid index

    return node;
}

void shift(PatternMatcher *handle, uint8_t instr)
{
    if (handle->current)
    {
        handle->current = handle->current->next[instr];
    }
    else
    {
        handle->current = newnode();
    }
    handle->depth++;
}

size_t reduce(PatternMatcher *handle, uint8_t *repr)
{
    size_t index;
    size_t depth;

    index = handle->current->index;
    depth = handle->depth;

    if (index)
    {
        assert(handle->current != &handle->root);
    }
    else
    {
        // TODO: make pattern
    }

    memcpy((void *)(repr - depth), handle->substitutions[index], depth);

    return 0;
}

static void _cleanup(struct NODE *node)
{
    for (size_t i = 0; i < _NODE_COUNT; i++)
        if (node->next[i])
            _cleanup(node->next[i]);

    free(node);
}

void cleanup(PatternMatcher *handle)
{
    for (size_t i = 1; i < _NODE_COUNT; i++)
        if (handle->root.next[i])
            _cleanup(handle->root.next[i]);

    free(handle->substitutions);
}

void make_pattern()
{
}

int main()
{
    PatternMatcher handle;

    init(&handle);

    cleanup(&handle);

    return 0;
}