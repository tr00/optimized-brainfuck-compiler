#ifndef __ALL_H_
#define __ALL_H_

#define VERSION "v0.00"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

enum OPCODE
{
    NOOP,

    INCR,
    DECR,
    NEXT,
    PREV,
    SKIP,
    LOOP,
    PUTC,
    GETC,

    // feel free to add any amount of extra instructions
    // but I need the first 9 to be in this order

    SETV,
    EXIT,

    VALDIFF, // tape[ptr] += arg1
    PTRDIFF, // ptr += arg1
    PERMUTE, // vectorized_add((vec_t *)tape, (vec_t *)arg1)
};


#define DST 0x04
#define SRC 0x02
#define ARG 0x01

#define isregister(reg, flag) ((reg) & (flag))

// incr  %dst, %src, $arg === r[%dst] += $arg * r[%src];
// some instructions might interpret the args differently
struct INSTR
{
    uint8_t opc : 5;
    uint8_t reg : 3;
    uint8_t dst, src, arg;
}; // 32bit

#define BB_MAX_INSTR 16

typedef struct BB
{
    size_t id;
    struct INSTR code[BB_MAX_INSTR];
    size_t offset, ptrdiff;
    struct BB *pred, *tail, *link;
} BasicBlock;

extern const char *opc2str[];

// unused
#define BFCHARS(EXP) \
        EXP == 43 || \
        EXP == 44 || \
        EXP == 45 || \
        EXP == 46 || \
        EXP == 60 || \
        EXP == 62 || \
        EXP == 91 || \
        EXP == 93

// frontend.c
#define SUCCESS 0
#define FAILURE 1
enum CMDFLAGS
{
    FLAG_EXEC,
    FLAG_VERSION,
    FLAG_HELP,
    FLAG_FILE
};

// scanner.c
BasicBlock *scan(char *source, size_t length);

// memutils.c
void *safe_malloc(size_t size);
void *safe_calloc(size_t size);
void *safe_realloc(void *ptr, size_t size);
// TODO: safe_ string.h functions
// TODO: safe_ mmap functions

// pattern-opt.c
#define _NODE_COUNT 9

typedef struct
{
    struct NODE
    {
        struct NODE *next[_NODE_COUNT];
        uint8_t *subst;
    } root;
    struct
    {
        struct NODE *current;
        size_t depth;
    };
} PatternMatcher;

void init(PatternMatcher *handle);
void cleanup(PatternMatcher *handle);
void shift(PatternMatcher *handle, uint8_t instr);
size_t reduce(PatternMatcher *handle, uint8_t *repr);

//
BasicBlock *allocBB(BasicBlock *pred);
void normalize(BasicBlock *bb);
void printBB(FILE *out, BasicBlock *bb);

#endif // everything needs to be above this
