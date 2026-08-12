#include <stdio.h>
#include <stdlib.h>
#include "processor.h"

int Register[REG_COUNT];

int PC;
int opcode;
int dest;
int src1;
int src2;
int end_of_simulation;

void reset(void)
{
    for (int i = 0; i < REG_COUNT; i++) {
        Register[i] = 0;
    }
    PC = 0;
    end_of_simulation = 0;
}

void fetch(void)
{
    /* Cast to unsigned char so values 0-255 are read correctly
       regardless of whether plain char is signed on this platform. */
    opcode = Instruction[PC];
    dest   = Instruction[PC + 1];
    src1   = Instruction[PC + 2];
    src2   = Instruction[PC + 3];

    PC = PC + 4;
}

void decode(void)
{
    /* Left empty */
}

void execute(void)
{
    switch (opcode) {
        case 1: /* Add */
            Register[dest] = Register[src1] + Register[src2];
            break;

        case 2: /* Subtract */
            Register[dest] = Register[src1] - Register[src2];
            break;

        case 3: /* Multiply */
            Register[dest] = Register[src1] * Register[src2];
            break;

        case 4: /* Divide */
            if (Register[src2] == 0) {
                printf("Error: division by zero at PC=%d\n", PC - 4);
                exit(1);
            }
            Register[dest] = Register[src1] / Register[src2];
            break;

        case 5: /* Read */
            Register[dest] = Data[src1];
            break;

        case 6: /* Write */
            Data[src1] = Register[dest];
            break;

        case 7: /* Move */
            Register[dest] = src1;
            break;

        case 0: /* End of program */
            end_of_simulation = 1;
            break;

        default:
            printf("Error: unknown opcode %d at PC=%d\n", opcode, PC - 4);
            exit(1);
    }
}