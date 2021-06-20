/**
 * UNUSED prolly gonna delete this
 */
#include <assert.h>

#include "all.h"

struct PATTERN
{
    size_t offset, ptrdiff, length;
    uint8_t *vector;
};

typedef struct
{
    struct CELL
    {
        struct CELL *values[256];
    };
} Patternizer;

/**
 * takes in a primitive instruction sequence and returns
 * stores that same sequence compressed as a PATTERN
 */
void patternize(struct PATTERN *dest, const uint8_t *sequence, size_t length)
{
    // normalization: [-<] == [<>-<]
    size_t ptrdiff = 0, offset = 0, maxptr = 0;
    for (size_t i = 0; i < length; i++)
    {
        if (sequence[i] == NEXT)
        {
            ptrdiff++;
            if (ptrdiff > maxptr)
                maxptr = ptrdiff;
        }
        else if (sequence[i] == PREV)
        {
            if (ptrdiff)
                ptrdiff--;
            else
                offset++;
        }
    }

    // compression: [<>-<] == [< { 0x00, 0xFF }]
    dest->vector = safe_calloc(offset + maxptr + 1);
    dest->ptrdiff = ptrdiff;
    dest->offset = offset;
    ptrdiff = offset;
    for (size_t i = 0; i < length; i++)
    {
        switch (sequence[i])
        {
        case NOOP:
            break;
        case INCR:
            dest->vector[ptrdiff]++;
            break;
        case DECR:
            dest->vector[ptrdiff]--;
            break;
        case NEXT:
            ptrdiff++;
            break;
        case PREV:
            assert(ptrdiff);
            ptrdiff--;
            break;
        default:
            assert(0);
        }
    }
}