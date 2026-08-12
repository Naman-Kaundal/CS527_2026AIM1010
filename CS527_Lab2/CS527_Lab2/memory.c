#include <stdio.h>
#include "memory.h"

char Instruction[256];
char Data[4096];

/* reads program.byte and data.byte (if present) and loads them into the
   Instruction and Data arrays. Files are stored as hex bytes, four per
   line, separated by space, no 0x prefix. Any bytes not covered by the
   files are left as 0 (default initial value of a global array). */
void initialize(void)
{
    FILE *fp;
    int a, b, c, d;
    int addr;

    for (addr = 0; addr < 256; addr++)
        Instruction[addr] = 0;
    for (addr = 0; addr < 4096; addr++)
        Data[addr] = 0;

    fp = fopen("program.byte", "r");
    if (fp != NULL) {
        addr = 0;
        while (addr < 256 && fscanf(fp, "%x %x %x %x", &a, &b, &c, &d) == 4) {
            Instruction[addr]     = (char) a;
            Instruction[addr + 1] = (char) b;
            Instruction[addr + 2] = (char) c;
            Instruction[addr + 3] = (char) d;
            addr += 4;
        }
        fclose(fp);
    }

    fp = fopen("data.byte", "r");
    if (fp != NULL) {
        addr = 0;
        while (addr < 4096 && fscanf(fp, "%x %x %x %x", &a, &b, &c, &d) == 4) {
            Data[addr]     = (char) a;
            Data[addr + 1] = (char) b;
            Data[addr + 2] = (char) c;
            Data[addr + 3] = (char) d;
            addr += 4;
        }
        fclose(fp);
    }
}

/* writes the final content of data memory back to data.byte */
void finalize(void)
{
    FILE *fp;
    int addr;

    fp = fopen("data.byte", "w");
    if (fp == NULL)
        return;

    for (addr = 0; addr < 4096; addr += 4) {
        fprintf(fp, "%X %X %X %X\n",
                (unsigned char) Data[addr],
                (unsigned char) Data[addr + 1],
                (unsigned char) Data[addr + 2],
                (unsigned char) Data[addr + 3]);
    }
    fclose(fp);
}

int mem_read32(int addr)
{
    unsigned char b0 = (unsigned char) Data[addr];
    unsigned char b1 = (unsigned char) Data[addr + 1];
    unsigned char b2 = (unsigned char) Data[addr + 2];
    unsigned char b3 = (unsigned char) Data[addr + 3];

    return (int) (b0 | (b1 << 8) | (b2 << 16) | (b3 << 24));
}

void mem_write32(int addr, int val)
{
    Data[addr]     = (char) (val & 0xFF);
    Data[addr + 1] = (char) ((val >> 8) & 0xFF);
    Data[addr + 2] = (char) ((val >> 16) & 0xFF);
    Data[addr + 3] = (char) ((val >> 24) & 0xFF);
}
