/**
 * 
 */
#include <assert.h>

#include "all.h"

/**
 * interpreter which has different modes based on optimization level
 * as well as code complexity.
 * 
 * INTERPRET:
 *      - runs the raw code in the simplest possible way
 *      - this mode is only used for sections which are
 *          guaranteed to only be executed once
 *      - only execution cycle with -O0
 * 
 * OPTIMIZE:
 *      - like interpret but it collects all sorts of information
 *          to optimize the code after the initial run
 *      - used for every loop which is not a comment (with -O1 or -O2)
 *      - more heavy optimizations when -O2 enabled
 * 
 * VIRTUAL:
 *      - similar as interpret but runs on optimized bytecode
 *      - has a richer instruction set (closer to the machine)
 *      - is the main execution cycle for -O1
 * 
 * TRACE:
 *      - like virtual but it collects the executed instructions
 *          and the branches taken and compiles it native machine code
 *      - similar to the optimize mode but for compilation not optimization
 * 
 * NATIVE:
 *      - executes dynamically compiled function snippets
 *      - partially used with -O2 with VIRTUAL as fallback mode (disabled on all other -Ox)
 *      - not yet implemented
 * 
 * assumes untouched string as input
 */
void interpret(char *source, size_t length)
{
    size_t depth;  // current loop depth
    uint8_t *tape; // tape array
    size_t cp;     // cell pointer
    size_t ip;     // instruction pointer

INTERPRET:
    for (ip = 0; ip < length; ip++)
    {
        switch (source[ip])
        {
        case '+':
            tape[cp]++;
            break;
        case '-':
            tape[cp]--;
            break;
        case '>':
            cp++;
            break;
        case '<':
            cp--;
            break;
        case '.':
            putc(tape[cp], stdout);
            break;
        case ',':
            tape[cp] = getc(stdin);
            break;
        case '[':
        {
            if (tape[cp])
            {
                // TODO: goto profiling
            }
            else
            {
                // we are dealing with a comment.
                // skip to the matching ] and continue interpreting
                do
                {
                    if (source[ip] == ']')
                    {
                        depth--;
                    }
                    else if (source[ip] == '[')
                    {
                        depth++;
                    }
                    ip++;
                } while (depth && ip < length);
            }
        }
        case ']':
            assert(0);
        default:
            continue;
        }
    }
OPTIMIZE:
VIRTUAL:
    for(; ip < length; ip++)
    {
        switch(source[ip])
        {
            case INCR:
            case SETV:
            case EXIT:
            default:
                assert(0);
        }
    }
TRACE:
NATIVE:
    return;
}

/*
    [->+>+<<] madd2

; normalized

    load ra, c0
    load rb, c1
    load rc, c2
    skip ra, L1
L0:
    incr ra, 0xFF
    incr rb, 0x01
    incr rc, 0x01
    loop ra, L0
L1:
    save ra, c0
    save rb, c1
    save rc, c2
    exit 0x00

; compressed

    load r32a, (32) c0
    skip ra, L1
L0:
    incr r32a, 0xFF010100
    loop ra, L0
L1:
    store r32a, (32) c0
    exit 0x00

; optimized

    load ra, c0
    load rb, c1
    load rc, c2
    incr rb, ra
    incr rc, ra
    setv ra, 0x00
    save ra, c0
    save rb, c1
    save rc, c2

    prev cp, 0x01
 ;; incr cp, 0x00

*/