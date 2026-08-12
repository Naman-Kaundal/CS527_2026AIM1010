#ifndef PROCESSOR_H
#define PROCESSOR_H

extern int Register[256];
extern int PC;

/* global condition flags */
extern int Z, N, C, V;

extern int end_of_simulation;

void reset(void);
void fetch(void);
void decode(void);
void execute(void);

#endif
