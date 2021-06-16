#ifndef __ALL_H_
#define __ALL_H_

enum INSTR
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

    SETV,
}

#endif

#define SUCCESS 0
#define FAILURE 1
#define BFCHARS(EXP) EXP == 43 || \
EXP == 44 || \
EXP == 45 || \
EXP == 46 || \
EXP == 60 || \
EXP == 62 || \
EXP == 91 || \
EXP == 93

