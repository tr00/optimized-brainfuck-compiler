/**
 * 
 */
#include "all.h"

void run(uint8_t *prog)
{

    uint8_t opcode;
    uint8_t *tape;
    size_t pc;

    pc = 0;

    while (1)
    {
        opcode = prog[pc];

        switch (opcode)
        {
        }

        pc++;
    }
}
