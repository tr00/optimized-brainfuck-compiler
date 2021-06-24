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

    cp = 0;
    sp = 0;

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

size_t invoke_trace(struct TRACE *trace, U8 *tape, size_t cp)
{

}

void interpret(const char *source)
{
    size_t ip; // instruction pointer 
    size_t cp; // cell pointer

    U8 *tape;

    // TODO: alloc & inits

    while(source[ip])
    {
        switch(source[ip])
        {
            case '[':
            {
                struct TRACE *trace;

                ip = optimize(trace, source, ip);

                cp = invoke_trace(trace, tape, cp);
            }
        }
    }
}

