#include <stdio.h>

#include "all.h"

#if USE_PATTERN_MATCHER

/**
 * Translates every brainfuck instruction
 * into the respective IR counterpart.
 * Every other character gets turned into a noop.
 * 
 * Every reducible expression, that is
 * an expression which consists of only instructions 
 * that change the value / ptr (including noop),
 * gets reduced to a compressed instruction
 * 
 * depending on the number of non-bf characters
 * there might be a large chunk of unused memory
 * at the end of the source buffer
 */
void scan(char *source, size_t length)
{
    PatternMatcher pm;

    init(&pm);

    for (size_t i = 0; i < length; i++)
    {
        switch (source[i])
        {
        case '+':
            source[i] = INCR;
            break;
        case '-':
            source[i] = DECR;
            break;
        case '<':
            source[i] = PREV;
            break;
        case '>':
            source[i] = NEXT;
            break;
        case '[':
            source[i] = SKIP;
            goto REDUCIBLE;
        case ']':
            source[i] = LOOP;
            goto REDUCIBLE;
        case '.':
            source[i] = PUTC;
            goto REDUCIBLE;
        case ',':
            source[i] = GETC;
            goto REDUCIBLE;
        default:
            source[i] = NOOP;
        }
        shift(&pm, source[i]);
        continue;
    REDUCIBLE:
        shift(&pm, source[i]);
        i -= reduce(&pm, source);
    }

    cleanup(&pm);
}
#elif 0

/**
 * plain non optimizing scanner
 */
void scan(char *source, size_t length)
{
    size_t i, j;
    for (i = 0, j = 0; i < length; i++, j++)
    {
        switch (source[i])
        {
        case '+':
            source[j] = INCR;
            break;
        case '-':
            source[j] = DECR;
            break;
        case '<':
            source[j] = PREV;
            break;
        case '>':
            source[j] = NEXT;
            break;
        case '[':
            source[j] = SKIP;
            break;
        case ']':
            source[j] = LOOP;
            break;
        case '.':
            source[j] = PUTC;
            break;
        case ',':
            source[j] = GETC;
            break;
        default:
            source[j--] = NOOP;
            break;
        }
    }

    for (; j < length; j++)
        source[j] = NOOP;
}

#else

BasicBlock *scan(char *source, size_t length)
{
    BasicBlock *root, *node;
    struct INSTR instr;
    size_t cp = 0;

    node = root = allocBB(NULL);

    instr.reg = 0;
    instr.src = 0;
    instr.dst = 0;

    for (size_t i = 0; i < length; i++)
    {
        switch (source[i])
        {
        case '+':
            instr.opc = INCR;
            instr.arg = 1;
            break;
        case '-':
            instr.opc = DECR;
            instr.arg = 1;
            break;
        default:
            continue;
        }

        if (cp == BB_MAX_INSTR)
        {
            BasicBlock *newnode = allocBB(node);
            node->link = newnode;
            node = newnode;
        }
    }
    return root;
}

#endif

// utility function
void printIR(const uint8_t *repr, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        switch (repr[i])
        {
        case INCR:
            printf("\tincr\n");
            break;
        case DECR:
            printf("\tdecr\n");
            break;
        case PREV:
            printf("\tprev\n");
            break;
        case NEXT:
            printf("\tnext\n");
            break;
        case SKIP:
            printf("\tskip\n");
            break;
        case LOOP:
            printf("\tloop\n");
            break;
        case PUTC:
            printf("\tputc\n");
            break;
        case GETC:
            printf("\tgetc\n");
            break;
        case NOOP:
            printf("\tnoop\n");
            break;
        default:
            printf("\t<unknown>\n");
            break;
        }
    }
}
