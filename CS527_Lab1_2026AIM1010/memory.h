#ifndef MEMORY_H
#define MEMORY_H

#define MEM_SIZE 256

/* Instruction holds the compiled bytecode program */
extern int Instruction[MEM_SIZE];

/* Data holds the program's working data */
extern int Data[MEM_SIZE];

/*
 * initialize() loads program.byte into Instruction[] and data.byte into Data[]
 */
void initialize(void);

/*
 * finalize()
 * Writes the current contents of Data[] back out to data.byte
 */
void finalize(void);

#endif