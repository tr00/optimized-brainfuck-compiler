#define _GNU_SOURCE

#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

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

void *safe_mmap(char *data, size_t size)
{
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if(ptr == MAP_FAILED)
    {
        // TODO: handle error
    }

    memcpy(ptr, data, size);
    if(mprotect(ptr, size, PROT_READ | PROT_EXEC) == -1)
    {
        // TODO: handle error
    }

    return ptr;
}

void safe_munmap(void *ptr, size_t size)
{
    if(munmap(ptr, size) == -1)
    {
        // TODO: handle error
    }
}
