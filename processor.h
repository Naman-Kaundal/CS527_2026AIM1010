#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdio.h>
#include "memory.h"

#define REG_COUNT 256
#define VREG_COUNT 32
#define VLANES 8

extern int Register[NP][REG_COUNT];
extern int VRegister[NP][VREG_COUNT][VLANES];

extern int PC[NP];
extern int opcode;
extern int dest;
extern int src1;
extern int src2;
extern int end_of_simulation[NP];

/* condition flags, one set per processor, updated by scalar add/subtract
 * only. A shared set of flags would let one process's add/sub silently affect another
 * process's branch, so each processor gets its own. */
extern int flagZ[NP];
extern int flagN[NP];
extern int flagC[NP];
extern int flagV[NP];

/* shared log file that the Print instruction writes to, opened once in
 * append mode the first time any processor is reset */
extern FILE *fd_log;

/*
 * reset()
 * Clears registers (scalar and vector), flags and end-of-simulation for
 * one processor, resets its PC, and makes sure the log file is open.
 */
void reset(int proc_id);

/*
 * fetch()
 * Translates the current PC of the given processor through
 * getPhysicalAddress() and reads the instruction bytes straight out of
 * physical memory into opcode/dest/src1/src2, then advances that
 * processor's PC by 4.
 */
void fetch(int proc_id);

/*
 * decode()
 * Left empty, kept for symmetry with reset/fetch/execute.
 */
void decode(int proc_id);

/*
 * execute()
 * Performs the operation indicated by opcode using dest, src1, src2 on
 * the given processor's registers.
 */
void execute(int proc_id);

/*
 * process_instructions()
 * Runs up to instruction_count fetch/decode/execute cycles for one
 * processor
 */
void process_instructions(int proc_id, int instruction_count);

#endif
