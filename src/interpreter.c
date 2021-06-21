/**
 * 
 */
#include <assert.h>

#include "all.h"
#include "configs.h"

/**
 * raw unoptimized interpreter
 */
void run_raw(U8 *source, size_t length)
{
    size_t ip, cp, sp;
    U8 *tape = safe_calloc(MAX_CELL_COUNT);
    size_t *labels = safe_malloc(MAX_LOOP_DEPTH * sizeof(size_t));

    for (ip = 0; ip < length; ip++)
    {
        switch (source[ip])
        {
        case INCR:
            tape[cp]++;
            break;
        case DECR:
            tape[cp]--;
            break;
        case PREV:
            cp--;
            break;
        case NEXT:
            cp++;
            break;
        case SKIP:
            if (tape[cp])
            {
                labels[sp++] = ip;
            }
            else
            {
                size_t depth = 1;
                do
                {
                    if(source[ip] == '[')
                    {
                        depth++;
                    }
                    else if(source[ip] == ']')
                    {
                        depth--;
                    }
                    ip++;
                } while (depth);
            }
            break;
        case LOOP:
            assert(sp);
            if (tape[cp])
            {
                ip = labels[sp];
            }
            else
            {
                sp--;
            }
            break;
        case PUTC:
            putc(tape[cp], stdout);
            fflush(stdout);
            break;
        case GETC:
            tape[cp] = getc(stdin);
            fflush(stdin);
            break;
        default:
            break;
        }
    }

    free(labels);
    free(tape);
}


/**
 * vm based execution (richer instruction set)
 */
void run_opt(U8 *source, size_t length)
{

}

void run_jit(U8 *source, size_t length)
{

}
