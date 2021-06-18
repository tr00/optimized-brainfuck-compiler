#include <stdlib.h>

#include "all.h"

void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);
    if(ptr == NULL)
    {
        // TODO: handle error
    }
    return ptr;
}

void *safe_calloc(size_t size)
{
    void *ptr = calloc(1, size);
    if(ptr == NULL)
    {
        // TODO: handle error
    }
    return ptr;
}

void *safe_realloc(void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if(ptr == NULL)
    {
        // TODO: handle error
    }
    return ptr;
}
