#ifndef __ALL_H_
#define __ALL_H_

#define VERSION "v0.00"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <immintrin.h>

typedef uint8_t U8;

typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef __m128i U128;
typedef __m256i U256;
typedef __m512i U512;


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

    SETZ, // tape[cp] = 0;
    SETV,
    EXIT,
};

struct INSTR
{
    uint8_t opc : 6;
    uint8_t dst : 2; // register
    uint8_t arg;
};

extern const char *opc2str[];

// unused
#define BFCHARS(EXP) \
    EXP == 43 ||     \
        EXP == 44 || \
        EXP == 45 || \
        EXP == 46 || \
        EXP == 60 || \
        EXP == 62 || \
        EXP == 91 || \
        EXP == 93

// error handling
#define throw(MSG, F...)         \
    {                            \
        fprintf(stderr, MSG, F); \
        exit(EXIT_FAILURE);      \
    }

// frontend.c
#define SUCCESS 0
#define FAILURE 1
enum CMDFLAGS
{
    FLAG_EXEC,
    FLAG_VERSION,
    FLAG_HELP,
    FLAG_FILE,
    FLAG_OUTFILE,
    FLAG_INTERPRET,
    FLAG_COMPILER,
    FLAG_INTERPRETER

    /*  */
};

// blocks.c
#define BB_MAX_INSTR 16


typedef struct BB
{
    U8 code[BB_MAX_INSTR];
    struct BB *pred;
    struct BB *next;
    size_t label;
} BasicBlock;

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
