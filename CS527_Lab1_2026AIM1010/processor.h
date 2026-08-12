#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "memory.h"

#define REG_COUNT 256

extern int Register[REG_COUNT];

extern int PC;
extern int opcode;
extern int dest;
extern int src1;
extern int src2;
extern int end_of_simulation;

/*
 * reset()
 * Clears all registers to zero and the end-of-simulation flag, resets the program counter 
 */
void reset(void);

/*
 * fetch()
 * Reads instruction at the current PC from Instruction[] into opcode/dest/src1/src2, then increments PC
 */
void fetch(void);

/*
 * decode()
 * Left empty
 */
void decode(void);

/*
 * execute()
 * Performs the operation indicated by opcode, using dest, src1 and src2 as operands.
 */
void execute(void);

#endif