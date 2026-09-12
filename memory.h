#ifndef MEMORY_H
#define MEMORY_H

/* number of processors the OS manages, fixed at compile time */
#define NP 4

/* physical memory and page size, both fixed at compile time */
#define MEMSIZE   8192
#define PAGESIZE  512

#define INSTR_LOGICAL_SIZE 1024
#define DATA_LOGICAL_SIZE  4096

#define NUM_INSTR_PAGES    (INSTR_LOGICAL_SIZE / PAGESIZE)          /* 2 */
#define NUM_DATA_PAGES     (DATA_LOGICAL_SIZE  / PAGESIZE)          /* 8 */
#define NUM_LOGICAL_PAGES  (NUM_INSTR_PAGES + NUM_DATA_PAGES)       /* 10 per process */

#define NUM_PHYSICAL_PAGES (MEMSIZE / PAGESIZE)                      /* 16, frame 0 reserved */

/* the single physical memory every process's logical addresses eventually
   resolve into */
extern unsigned char memory[MEMSIZE];

/*
 * getPhysicalAddress()
 * Translates a logical address for proc_id into a physical one.
 */
int getPhysicalAddress(int proc_id, int isFetch, int address);

/*
 * initialize()
 * Loads program_byte_path and data_byte_path for proc_id: works out how
 * many pages each file actually needs, grabs that many free frames,
 * records them in that process's page table, and copies the bytes into
 * physical memory page by page (zero-padding the rest of the last page).
 */
void initialize(int proc_id, const char *program_byte_path, const char *data_byte_path);

/*
 * finalize()
 * Writes every data page allocated to proc_id back out to data_byte_path,
 * then returns all of that process's pages (instruction and data) to the
 * free list and clears its page table.
 */
void finalize(int proc_id, const char *data_byte_path);

/* read_word() / write_word() go through getPhysicalAddress(proc_id, 0, ...)
   to reach a 32 bit big endian value in physical memory. */
int read_word(int proc_id, int addr);
void write_word(int proc_id, int addr, int value);

#endif
