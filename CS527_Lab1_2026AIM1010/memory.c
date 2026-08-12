#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

/* Definitions of the memory arrays declared extern in memory.h */
int Instruction[MEM_SIZE];
int Data[MEM_SIZE];

void initialize(void)
{
    FILE *fp;
    int value;
    int index;

    /* Load instruction memory from program.byte */
    fp = fopen("program.byte", "r");
    if (fp == NULL) {
        printf("Error: could not open program.byte\n");
        exit(1);
    }

    index = 0;
    while (index < MEM_SIZE && fscanf(fp, "%d", &value) == 1) {
        Instruction[index] = value;
        index++;
    }
    fclose(fp);

    /* Load data memory from data.byte
     * Each value read is stored at a word-aligned address to match the addresses used by
     * Read/Write instructions in the source program */
    fp = fopen("data.byte", "r");
    if (fp == NULL) {
        printf("Error: could not open data.byte\n");
        exit(1);
    }

    int addr = 0;
    while (addr < MEM_SIZE && fscanf(fp, "%d", &value) == 1) {
        Data[addr] = value;
        addr += 4;
    }
    fclose(fp);
}

void finalize(void)
{
    FILE *fp = fopen("data.byte", "w");
    if (fp == NULL) {
        printf("Error: could not write data.byte\n");
        exit(1);
    }

    for (int addr = 0; addr < MEM_SIZE; addr += 4) {
        fprintf(fp, "%d\n", Data[addr]);
    }

    fclose(fp);
}