#ifndef MEMORY_H
#define MEMORY_H

/* instruction memory is 256 bytes, data memory is 4096 bytes (Lab2 change) */
extern char Instruction[256];
extern char Data[4096];

void initialize(void);
void finalize(void);

/* helper functions to read/write a 32 bit value from data memory,
   little endian, used by processor.c for memory read/write instructions */
int mem_read32(int addr);
void mem_write32(int addr, int val);

#endif
