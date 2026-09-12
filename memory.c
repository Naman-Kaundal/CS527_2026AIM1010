#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

unsigned char memory[MEMSIZE];

static unsigned char pageTable[NP][NUM_LOGICAL_PAGES];

/* 0 = free, 1 = allocated */
static unsigned char freePages[NUM_PHYSICAL_PAGES];

/* how many data pages proc_id currently owns, so finalize() knows exactly
   how much to write back out and how many pages to free */
static int data_page_count[NP];

static int frames_ready = 0;

static void ensure_frames_ready(void)
{
    if (frames_ready) return;
    for (int i = 0; i < NUM_PHYSICAL_PAGES; i++) freePages[i] = 0;
    freePages[0] = 1;   /* frame 0 is reserved, never allocated */
    frames_ready = 1;
}

static int getFreePage(void)
{
    ensure_frames_ready();
    for (int i = 0; i < NUM_PHYSICAL_PAGES; i++) {
        if (!freePages[i]) {
            freePages[i] = 1;
            return i;
        }
    }
    printf("Error: out of physical memory, no free page available\n");
    exit(1);
}

int getPhysicalAddress(int proc_id, int isFetch, int address)
{
    int limit = isFetch ? INSTR_LOGICAL_SIZE : DATA_LOGICAL_SIZE;
    if (address < 0 || address >= limit) {
        printf("Error: logical address %d out of range for proc %d (%s)\n",
               address, proc_id, isFetch ? "instruction" : "data");
        exit(1);
    }

    int index = isFetch ? (address / PAGESIZE) : (address / PAGESIZE + NUM_INSTR_PAGES);
    int frame = pageTable[proc_id][index];

    if (frame == 0) {
        printf("Error: page fault, proc %d touched an unmapped %s page (logical address %d)\n",
               proc_id, isFetch ? "instruction" : "data", address);
        exit(1);
    }

    return frame * PAGESIZE + (address % PAGESIZE);
}

static int ceil_div(int a, int b)
{
    return (a + b - 1) / b;
}

/* reads a hex-byte file (four bytes per line, as always) into buf,
   returns how many bytes it actually contained */
static int load_hex_file(const char *path, unsigned char *buf, int max_bytes)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: could not open %s\n", path);
        exit(1);
    }

    int count = 0;
    unsigned int value;
    while (count < max_bytes && fscanf(fp, "%x", &value) == 1) {
        buf[count++] = (unsigned char)value;
    }
    fclose(fp);
    return count;
}

void initialize(int proc_id, const char *program_byte_path, const char *data_byte_path)
{
    ensure_frames_ready();

    for (int i = 0; i < NUM_LOGICAL_PAGES; i++) pageTable[proc_id][i] = 0;

    static unsigned char temp[DATA_LOGICAL_SIZE];  /* big enough for either file */

    /* ---- program.byte -> instruction pages, logical page index 0.. ---- */
    int program_bytes = load_hex_file(program_byte_path, temp, INSTR_LOGICAL_SIZE);
    int pages = ceil_div(program_bytes, PAGESIZE);
    if (pages == 0) pages = 1;
    if (pages > NUM_INSTR_PAGES) {
        printf("Error: %s does not fit in the instruction address space\n", program_byte_path);
        exit(1);
    }
    for (int p = 0; p < pages; p++) {
        int frame = getFreePage();
        pageTable[proc_id][p] = (unsigned char)frame;
        int base = frame * PAGESIZE;
        for (int b = 0; b < PAGESIZE; b++) {
            int logical = p * PAGESIZE + b;
            memory[base + b] = (logical < program_bytes) ? temp[logical] : 0;
        }
    }

    /* ---- data.byte -> data pages, logical page index NUM_INSTR_PAGES.. ---- */
    int data_bytes = load_hex_file(data_byte_path, temp, DATA_LOGICAL_SIZE);
    int dpages = ceil_div(data_bytes, PAGESIZE);
    if (dpages == 0) dpages = 1;
    if (dpages > NUM_DATA_PAGES) {
        printf("Error: %s does not fit in the data address space\n", data_byte_path);
        exit(1);
    }
    for (int p = 0; p < dpages; p++) {
        int frame = getFreePage();
        pageTable[proc_id][NUM_INSTR_PAGES + p] = (unsigned char)frame;
        int base = frame * PAGESIZE;
        for (int b = 0; b < PAGESIZE; b++) {
            int logical = p * PAGESIZE + b;
            memory[base + b] = (logical < data_bytes) ? temp[logical] : 0;
        }
    }
    data_page_count[proc_id] = dpages;
}

void finalize(int proc_id, const char *data_byte_path)
{
    int total_bytes = data_page_count[proc_id] * PAGESIZE;

    FILE *fp = fopen(data_byte_path, "w");
    if (fp == NULL) {
        printf("Error: could not write %s\n", data_byte_path);
        exit(1);
    }

    for (int addr = 0; addr < total_bytes; addr += 4) {
        int phys = getPhysicalAddress(proc_id, 0, addr);
        fprintf(fp, "%02X %02X %02X %02X\n",
                memory[phys], memory[phys + 1], memory[phys + 2], memory[phys + 3]);
    }
    fclose(fp);

    /* return every page (instruction and data) this process was using */
    for (int i = 0; i < NUM_LOGICAL_PAGES; i++) {
        if (pageTable[proc_id][i] != 0) {
            freePages[pageTable[proc_id][i]] = 0;
            pageTable[proc_id][i] = 0;
        }
    }
    data_page_count[proc_id] = 0;
}

int read_word(int proc_id, int addr)
{
    int phys = getPhysicalAddress(proc_id, 0, addr);
    unsigned int b0 = memory[phys];
    unsigned int b1 = memory[phys + 1];
    unsigned int b2 = memory[phys + 2];
    unsigned int b3 = memory[phys + 3];
    return (int)((b0 << 24) | (b1 << 16) | (b2 << 8) | b3);
}

void write_word(int proc_id, int addr, int value)
{
    int phys = getPhysicalAddress(proc_id, 0, addr);
    unsigned int uv = (unsigned int)value;
    memory[phys]     = (unsigned char)((uv >> 24) & 0xFF);
    memory[phys + 1] = (unsigned char)((uv >> 16) & 0xFF);
    memory[phys + 2] = (unsigned char)((uv >> 8) & 0xFF);
    memory[phys + 3] = (unsigned char)(uv & 0xFF);
}
