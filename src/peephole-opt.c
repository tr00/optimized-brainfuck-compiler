#include <stdio.h>

#include "all.h"

BasicBlock *allocBB(BasicBlock *pred)
{
    static size_t id = 0;

    BasicBlock *bb = safe_malloc(sizeof(BasicBlock));

    bb->id = id++;
    bb->pred = pred;
    bb->tail = NULL;
    bb->link = NULL;
    bb->ptrdiff = 0;

    for (size_t i = 0; i < BB_MAX_INSTR; i++)
        bb->code[i].opc = NOOP;

    return bb;
}

/**
 * 
 * 
 */
void normalize(BasicBlock *bb)
{
    size_t offset;
    size_t ptrdiff;
    size_t maxptrdiff;

    offset = ptrdiff = maxptrdiff = 0;

    for (size_t i = 0; i < BB_MAX_INSTR; i++)
    {
        switch (bb->code[i].opc)
        {
        case NEXT:
        {
            ptrdiff++;
            if (ptrdiff > maxptrdiff)
                maxptrdiff = ptrdiff;

            continue;
        }
        case PREV:
        {
            if (ptrdiff)
                ptrdiff--;
            else
                offset++;

            continue;
        }
        case SKIP:
        case LOOP:
            break;
        default:
            continue;
        }
    }

    bb->ptrdiff = ptrdiff;
    bb->offset = offset;
    ptrdiff = offset;

    for (size_t i = 0, j = 0; i < BB_MAX_INSTR; i++)
    {
        switch (bb->code[i].opc)
        {
        case DECR:
            bb->code[i].opc = INCR;
            bb->code[i].arg = -bb->code[i].arg;
        case INCR:
        case SETV:
        case PUTC:
        case GETC:
        {
            if (i != j)
            {
                bb->code[j] = bb->code[i];
                bb->code[i] = (struct INSTR){
                    .opc = NOOP,
                };
            }
            bb->code[j].src = bb->code[j].dst = ptrdiff;
            j++;
            break;
        }
        case PREV:
            ptrdiff--;
            bb->code[i].opc = NOOP;
            break;
        case NEXT:
            ptrdiff++;
            bb->code[i].opc = NOOP;
            break;
        default:
            break;
        }
    }
}

/**
 * null sequence + combine operations
 * 
 */
void peepopt(BasicBlock *bb)
{
    for (size_t i = 0; i < BB_MAX_INSTR; i++)
    {
    }
}

void printBB(FILE *out, BasicBlock *bb)
{
    struct INSTR p;

    fprintf(out, "BB%ul(offset = %u)\n", bb->id, bb->offset);
    for (size_t i = 0; i < BB_MAX_INSTR; i++)
    {
        p = bb->code[i];
        switch (p.opc)
        {
        case INCR:
            fprintf(out, "\tincr  %%r%u, %%r%u, $%u\n", p.dst, p.src, p.arg);
            break;
        case SETV:
            fprintf(out, "\tsetv  %%r%u, %%r%u, $%u\n", p.dst, p.src, p.arg);
            break;
        case PUTC:
        {
            if (isregister(p.reg, SRC))
            {
                fprintf(out, "\tputc  %%r%u, $%u\n", p.src, p.arg);
            }
            else
            {
                fprintf(out, "\tputc $%u, $%u\n", p.src, p.arg);
            }
            break;
        }
        case GETC:
            fprintf(out, "\tgetc  %%r%u\n", p.dst);
            break;
        case NOOP:
            fprintf(out, "\tnoop\n");
            break;
        default:
            fprintf(out, "\t<unknown:%u>\n", p.opc);
            break;
        }
    }
    fprintf(out, "\n");
}
