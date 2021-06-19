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

    instr.opc = NOOP;
    instr.reg = 0;
    instr.src = 0;
    instr.dst = 0;
    instr.arg = 0;

    for (size_t i = 0; i < length; i++)
    {
        switch (source[i])
        {
        case '+':
            instr.opc = INCR;
            break;
        case '-':
            instr.opc = DECR;
            break;
        case '<':
            instr.opc = PREV;
            break;
        case '>':
            instr.opc = NEXT;
            break;
        case '.':
            instr.opc = PUTC;
            break;
        case ',':
            instr.opc = GETC;
            break;
        case '[':
            instr.opc = SKIP;
            goto NEWBLOCK;
        case ']':
            instr.opc = LOOP;
            goto NEWBLOCK;
        default:
            continue;
        }

        instr.arg = 1;
        char tok = source[i];

        while (source[++i] == tok)
        {
            instr.arg++;
        }
        i--;

        if (cp == BB_MAX_INSTR)
        NEWBLOCK:
        {
            BasicBlock *newnode = allocBB(node);
            node->link = newnode;
            node = newnode;
            cp = 0;
        }

            node->code[cp++] = instr;
    }
    return root;
}

#endif