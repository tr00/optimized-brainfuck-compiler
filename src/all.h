#ifndef __ALL_H_
#define __ALL_H_

#define VERSION "v0.00"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
// #include <immintrin.h>

typedef uint8_t U8;

typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

// typedef __m128i U128;
// typedef __m256i U256;
// typedef __m512i U512;

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

    SETZ, // tape[cp] = 0;
    SETV,

    FNZL,
    FNZR,
    EXIT,
};

struct NODE;

struct INSTR
{
    U8 opc;
    U8 idx;
    U8 arg;
    U8 : 8;
};

struct LOOP
{
    struct INSTR *arr;
};

struct TRACE
{
    struct NODE *nodes;
};

struct NODE
{
    /**
     * we could pack these bits into the
     * pointers but thats risky so this is fine for now
     */
    enum _NODE_FLAG
    {
        NODE_FLAG_LOOP,
        NODE_FLAG_INSTR,
        NODE_FLAG_TRACE,
    } flag;
    union _NODE_DATA
    {
        struct LOOP *loop;
        struct INSTR *instr;
        struct TRACE *trace;
    };
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

// scanner.c
U8 *scan(char *source, size_t length);

// memutils.c
void *safe_malloc(size_t size);
void *safe_calloc(size_t size);
void *safe_realloc(void *ptr, size_t size);
void *safe_mmap(const char *data, size_t size);
void safe_munmap(void *ptr, size_t size);
// TODO: safe_ string.h functions

// interpreter.c
void interpret(const char *source);

size_t optimize(struct TRACE *dest, const char *source, size_t ip);

#endif // everything needs to be above this
