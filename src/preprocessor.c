/**
 * the preprocessor takes care of repeated instructions,
 * comments, unused space & labels
 */
#include <stdio.h>

#include "all.h"
#include "configs.h"

/**
 * plain non optimizing scanner
 */
U8 *scan(char *source, size_t length)
{
    size_t i, j, k;
    size_t capacity = length / 2;

    U8 *output = safe_malloc(capacity);
    size_t *labels = safe_malloc(MAX_LOOP_DEPTH * sizeof(size_t));

    i = j = k = 0;

    while (i < length)
    {
        switch (source[i++])
        {
        case '+':
            output[j++] = INCR;
            output[j] = 0x01;
            break;
        case '-':
            output[j++] = INCR;
            output[j] = 0xFF;
            break;
        case '<':
            output[j++] = PREV;
            continue;
        case '>':
            output[j++] = NEXT;
            continue;
        case '[':
        {
            output[j] = SKIP;
            labels[k++] = j;
            j += 1 + sizeof(size_t);
            continue;
        }
        case ']':
        { // i know this is unreadable :|
            output[j++] = LOOP;
            size_t lbl = labels[--k];
            *(size_t *)(&output[j]) = lbl + sizeof(size_t);
            *(size_t *)(&output[lbl + 1]) = j + sizeof(size_t) - 1;
            j += sizeof(size_t);
            continue;
        }
        case '.':
            output[j++] = PUTC;
            continue;
        case ',':
            output[j++] = GETC;
            continue;
        default:
            continue;
        }

        while (1)
        {
            if (source[i] == '+')
            {
                output[j]++;
            }
            else if (source[i] == '-')
            {
                output[j]--;
            }
            i++;
        }
        j++;
    }

    return output;
}