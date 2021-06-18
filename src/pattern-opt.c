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

static struct NODE *newnode()
{
    struct NODE *node = safe_malloc(sizeof(struct NODE));

    node->next[NOOP] = node;
    for (size_t i = 1; i < _NODE_COUNT; i++)
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
    struct NODE *root;
    struct NODE *current;
    uint8_t *start;
    size_t depth;

    depth = handle->depth;
    current = handle->current;
    root = &handle->root;
    start = (uint8_t *)(repr - depth);

    // repeated noop's
    if (current == root)
        return depth;

    // need new subst
    if (!current->subst)
    {
        // TODO: make subst
        assert(0);
    }

    memcpy((void *)start, current->subst, depth);

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

