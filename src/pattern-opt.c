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
 * (I will prolly optimize this to a radix tree in the future)
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
#include <stdio.h>

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
        uint8_t *subst;
    } root;
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

static struct NODE *newnode()
{
    struct NODE *node = safe_malloc(sizeof(struct NODE));

    for (size_t i = 0; i < _NODE_COUNT; i++)
        node->next[i] = NULL;

    node->subst = NULL;

    return node;
}

static void insert(PatternMatcher *handle, size_t length, uint8_t *pattern, uint8_t *subst)
{
    struct NODE *node = &handle->root;

    for (size_t i = 0; i < length; i++)
    {
        if (!node->next[pattern[i]])
        {
            node->next[pattern[i]] = newnode();
        }

        node = node->next[pattern[i]];
    }

    if (!node->subst)
    {
        node->subst = subst;
    }
}

void init(PatternMatcher *handle)
{
    handle->root.next[NOOP] = &handle->root;
    for (size_t i = 1; i < _NODE_COUNT; i++)
        handle->root.next[i] = NULL;

    handle->root.subst = NULL;
    handle->current = &handle->root;
    handle->depth = 0;

#define STACK(N, E...) ((uint8_t[N]){E})
#define HEAP(N, E...) memcpy(safe_malloc(N), STACK(N, E), N)

    insert(handle, 2, STACK(2, SKIP, LOOP), HEAP(2, NOOP, NOOP));

#undef STACK
#undef HEAP
}

void shift(PatternMatcher *handle, uint8_t instr)
{
    if (!handle->current->next[instr])
        handle->current->next[instr] = newnode();

    handle->current = handle->current->next[instr];
    handle->depth++;
}

size_t reduce(PatternMatcher *handle, uint8_t *repr)
{
    size_t depth = handle->depth;

    if (!handle->current->subst)
    {
        assert(0);
        // TODO: make pattern
    }

    memcpy((void *)(repr - depth), handle->current->subst, depth);

    return 0;
}

static void _cleanup(struct NODE *node)
{
    for (size_t i = 0; i < _NODE_COUNT; i++)
        if (node->next[i])
            _cleanup(node->next[i]);

    if (node->subst)
        free(node->subst);
    free(node);
}

void cleanup(PatternMatcher *handle)
{
    for (size_t i = 1; i < _NODE_COUNT; i++)
        if (handle->root.next[i])
            _cleanup(handle->root.next[i]);
}

void make_pattern()
{
}

int main()
{
    PatternMatcher handle;

    init(&handle);

    uint8_t *repr = malloc(2);
    size_t sp = 0;

    shift(&handle, SKIP);
    shift(&handle, LOOP);
    reduce(&handle, repr + 2);

    printf("%u %u\n", repr[0], repr[1]);

    free(repr);
    cleanup(&handle);

    return 0;
}
