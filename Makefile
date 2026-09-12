CC = gcc
CFLAGS = -Wall -std=c99

ifeq ($(OS),Windows_NT)
    TARGET = simulator.exe
else
    TARGET = simulator
endif

all: $(TARGET)

$(TARGET): main.c compiler.c processor.c memory.c os.c compiler.h processor.h memory.h os.h
	$(CC) $(CFLAGS) main.c compiler.c processor.c memory.c os.c -o $(TARGET)

clean:
	rm -f simulator simulator.exe compiled_*.byte os_log.txt

# Usage:
#   ./simulator program.txt data.byte       (Linux/macOS/WSL)
#   ./simulator array_add.txt array_add_data.byte
#   .\simulator.exe [<program.txt> <data.byte>]     (Windows PowerShell)
#
#   Two tasks at once, non-interactive:
#   (echo "fir.txt fir_data.byte"; echo exit) | ./simulator array_add.txt 
#   array_add_data.byte
#
#   Two tasks at once, interactive
#   ./simulator array_add.txt array_add_data.byte
#   fir.txt fir_data.byte
#   exit
#






















# Starting a task this way is optional - the OS also accepts new tasks
# from the shell prompt while it is running, one line at a time:
#   $ array_add.txt array_add_data.byte
# Typing "exit" stops the shell from taking any more new tasks; tasks
# already running are left to finish. Print instructions are logged to
# os_log.txt (opened in append mode, so old runs are not overwritten).
#
# To run the array add test:
#   ./simulator array_add.txt array_add_data.byte
#   (then type "exit" at the $ prompt, or pipe it in: see below)
#
# To run the FIR filter test:
#   ./simulator fir.txt fir_data.byte
#
# To run both at once (they land on separate processors) without typing
# anything interactively:
#   (echo fir.txt fir_data.byte; echo exit) | ./simulator array_add.txt array_add_data.byte









--------------------------------------------------------------Extension 1
Single Processor with Multitasking and Context Switching

memory.h
#define NP 1

processor.h
#define VLANES 8
typedef struct {
    int Register[REG_COUNT];
    int VRegister[VREG_COUNT][VLANES];

    int PC;

    int flagZ;
    int flagN;
    int flagC;
    int flagV;

    int end_of_simulation;
} CPUContext;
void save_context(int proc_id, CPUContext *context);
void restore_context(int proc_id, CPUContext *context);


processor.c
after reset()

void save_context(int proc_id, CPUContext *context)
{
    for (int i = 0; i < REG_COUNT; i++)
        context->Register[i] = Register[proc_id][i];

    for (int i = 0; i < VREG_COUNT; i++)
        for (int j = 0; j < VLANES; j++)
            context->VRegister[i][j] = VRegister[proc_id][i][j];

    context->PC = PC[proc_id];

    context->flagZ = flagZ[proc_id];
    context->flagN = flagN[proc_id];
    context->flagC = flagC[proc_id];
    context->flagV = flagV[proc_id];

    context->end_of_simulation = end_of_simulation[proc_id];
}

void restore_context(int proc_id, CPUContext *context)
{
    for (int i = 0; i < REG_COUNT; i++)
        Register[proc_id][i] = context->Register[i];

    for (int i = 0; i < VREG_COUNT; i++)
        for (int j = 0; j < VLANES; j++)
            VRegister[proc_id][i][j] = context->VRegister[i][j];

    PC[proc_id] = context->PC;

    flagZ[proc_id] = context->flagZ;
    flagN[proc_id] = context->flagN;
    flagC[proc_id] = context->flagC;
    flagV[proc_id] = context->flagV;

    end_of_simulation[proc_id] = context->end_of_simulation;
}

os.c(change)
typedef struct {
    int  pid;
    int  proc_id;
    int  finished;

    char program_path[128];
    char data_path[128];

    CPUContext context;
    int context_valid;
} Task;


loader()
t->finished = 0;
t->proc_id = -1;
t->context_valid = 0;




start_task_on_processor()
initialize(proc_id, bytecode_path, t->data_path);

if (t->context_valid) {
    restore_context(proc_id, &t->context);
}
else {
    reset(proc_id);
    t->context_valid = 1;
}


scheduler
static int current_task = 0;








------------------------------------------------------------Extension 1a
Extend Simple Multitasking Processor to Multiprocessor

memory.h
#define NP 4

processor.h
extern int Register[NP][REG_COUNT];
extern int VRegister[NP][VREG_COUNT][VLANES];

extern int PC[NP];



processor.c
int Register[NP][REG_COUNT];
int VRegister[NP][VREG_COUNT][VLANES];
int PC[NP];


os.c
static void print_processor_status(void)
{
    for (int p = 0; p < NP; p++) {
        if (proc_busy[p])
            printf("Processor %d : BUSY\n", p);
        else
            printf("Processor %d : FREE\n", p);
    }
}

print_processor_status();








---------------------------------------------------Extension 2(Priority)
Advanced CPU Scheduling
os.c
typedef struct {
    int pid;
    int proc_id;
    int finished;

    int priority;

    char program_path[128];
    char data_path[128];
} Task;







loader()
t->proc_id = -1;
t->priority = 1;



os.c
static int find_highest_priority_task(void)
{
    int best = -1;

    for (int i = 0; i < task_count; i++) {

        if (tasks[i].finished)
            continue;

        if (tasks[i].proc_id == -1)
            continue;

        if (best == -1 ||
            tasks[i].priority < tasks[best].priority) {

            best = i;
        }
    }

    return best;
}




--------------------------------------------------------Extension 2(SJF)
Advanced CPU Scheduling

Task
int estimated_time;
static int find_shortest_task(void)
{
    int best = -1;

    for (int i = 0; i < task_count; i++) {

        if (tasks[i].finished)
            continue;

        if (tasks[i].proc_id == -1)
            continue;

        if (best == -1 ||
            tasks[i].estimated_time < tasks[best].estimated_time) {

            best = i;
        }
    }

    return best;
}




--------------------------------------------------Extension 2(MLF Queue)
Advanced CPU Scheduling
Task
int queue_level;
int dynamic_priority;
int executed_instructions;


#define HIGH_QUEUE    0
#define MEDIUM_QUEUE  1
#define LOW_QUEUE     2

if (t->executed_instructions >= TIME_SLICE)
    t->queue_level++;


New Files:
scheduler.c
find_highest_priority_task()
find_shortest_task()
select_next_task()


scheduler.h








-------------------------------------------------------------Extension 3
Page Table Stored in Physical Memory
memory.h
#define PAGE_TABLE_FRAME_COUNT 1
#define PAGE_TABLE_BASE_FRAME  1


Page Table
#define PAGE_TABLE_BYTES (NP * NUM_LOGICAL_PAGES)

memory.c
before getPhysicalAddress()
static int page_table_address(int proc_id, int logical_page)
{
    return PAGE_TABLE_BASE_FRAME * PAGESIZE
           + proc_id * NUM_LOGICAL_PAGES
           + logical_page;
}
static unsigned char get_page_table_entry(int proc_id, int logical_page)
{
    int addr = page_table_address(proc_id, logical_page);

    return memory[addr];
}
static void set_page_table_entry(int proc_id,
                                 int logical_page,
                                 unsigned char frame)
{
    int addr = page_table_address(proc_id, logical_page);

    memory[addr] = frame;
}




change getPhysicalAddress()
int frame = get_page_table_entry(proc_id, index);

Change initialization
for (int i = 0; i < NUM_LOGICAL_PAGES; i++)
    set_page_table_entry(proc_id, i, 0);

Change page allocation
set_page_table_entry(proc_id, p, (unsigned char)frame);
within it:
set_page_table_entry(
    proc_id,
    NUM_INSTR_PAGES + p,
    (unsigned char)frame
);

change finalize
unsigned char frame =
    get_page_table_entry(proc_id, i);

if (frame != 0) {
    freePages[frame] = 0;
    set_page_table_entry(proc_id, i, 0);
}

change
freePages[0] = 1;
freePages[1] = 1;




-------------------------------------------------------------Extension 4
Memory-Mapped I/O
io.c
#include <stdio.h>
#include "io.h"

int io_read(int address)
{
    switch (address) {

        case IO_KEYBOARD:
            return getchar();

        case IO_TIMER:
            return 0;

        default:
            printf("Error: invalid I/O read address %d\n", address);
            return 0;
    }
}

void io_write(int address, int value)
{
    switch (address) {

        case IO_CONSOLE:
            printf("%c", value);
            fflush(stdout);
            break;

        default:
            printf("Error: invalid I/O write address %d\n", address);
            break;
    }
}


io.h
#ifndef IO_H
#define IO_H

#define IO_BASE       2048
#define IO_CONSOLE    2048
#define IO_KEYBOARD   2052
#define IO_TIMER      2056

int io_read(int address);
void io_write(int address, int value);

#endif

memory.c
#include "io.h"


read word():
int read_word(int proc_id, int addr)
{
    if (addr >= IO_BASE)
        return io_read(addr);

    int phys = getPhysicalAddress(proc_id, 0, addr);

    unsigned int b0 = memory[phys];
    unsigned int b1 = memory[phys + 1];
    unsigned int b2 = memory[phys + 2];
    unsigned int b3 = memory[phys + 3];

    return (int)((b0 << 24) |
                 (b1 << 16) |
                 (b2 << 8) |
                 b3);
}


write word():
void write_word(int proc_id, int addr, int value)
{
    if (addr >= IO_BASE) {
        io_write(addr, value);
        return;
    }

    int phys = getPhysicalAddress(proc_id, 0, addr);

    unsigned int uv = (unsigned int)value;

    memory[phys]     = (unsigned char)((uv >> 24) & 0xFF);
    memory[phys + 1] = (unsigned char)((uv >> 16) & 0xFF);
    memory[phys + 2] = (unsigned char)((uv >> 8) & 0xFF);
    memory[phys + 3] = (unsigned char)(uv & 0xFF);
}



------------------------------------------------------------Extension 5
Memory Map and Process Performance Utilities
utilities.h
#ifndef UTILITIES_H
#define UTILITIES_H

void show_processes(void);
void show_memory_map(void);
void show_statistics(void);

#endif


utilities.c
#include <stdio.h>
#include "utilities.h"

void show_processes(void)
{
    printf("\n=== PROCESS TABLE ===\n");

    printf("PID\tSTATUS\tPROCESSOR\n");
    printf("-----------------------------\n");

    /*
     * Connect this function to the OS task table.
     */
}


os.h
void show_processes(void);

os.c
void show_processes(void)
{
    printf("\n=== PROCESS TABLE ===\n");

    printf("PID\tProcessor\tStatus\n");
    printf("-----------------------------\n");

    for (int i = 0; i < task_count; i++) {

        printf("%d\t", tasks[i].pid);

        if (tasks[i].finished)
            printf("-\t\tFINISHED\n");
        else if (tasks[i].proc_id == -1)
            printf("-\t\tWAITING\n");
        else
            printf("%d\t\tRUNNING\n",
                   tasks[i].proc_id);
    }
}


memory.h
void show_memory_map(void);

memory.c
void show_memory_map(void)
{
    printf("\n=== PHYSICAL MEMORY ===\n");

    for (int frame = 0;
         frame < NUM_PHYSICAL_PAGES;
         frame++) {

        printf("Frame %d : %s\n",
               frame,
               frame == 0 ? "RESERVED" : "ALLOCATED/FREE");
    }
}



Task
long instructions_executed;
long scheduler_ticks;

**t->instructions_executed += TIME_SLICE;





----------------------------------------------------------------Extension 6
Cache, TLB and Timing
tlb.h
#ifndef TLB_H
#define TLB_H

#define TLB_SIZE 4

void tlb_init(void);
int tlb_lookup(int proc_id, int logical_page);
void tlb_insert(int proc_id, int logical_page, int frame);
void tlb_flush(void);

#endif


tlb.c
#include "tlb.h"

typedef struct {
    int valid;
    int proc_id;
    int logical_page;
    int frame;
} TLBEntry;

static TLBEntry tlb[TLB_SIZE];

void tlb_init(void)
{
    for (int i = 0; i < TLB_SIZE; i++)
        tlb[i].valid = 0;
}

int tlb_lookup(int proc_id, int logical_page)
{
    for (int i = 0; i < TLB_SIZE; i++) {

        if (tlb[i].valid &&
            tlb[i].proc_id == proc_id &&
            tlb[i].logical_page == logical_page) {

            return tlb[i].frame;
        }
    }

    return -1;
}

void tlb_insert(int proc_id,
                int logical_page,
                int frame)
{
    static int next = 0;

    tlb[next].valid = 1;
    tlb[next].proc_id = proc_id;
    tlb[next].logical_page = logical_page;
    tlb[next].frame = frame;

    next = (next + 1) % TLB_SIZE;
}

void tlb_flush(void)
{
    for (int i = 0; i < TLB_SIZE; i++)
        tlb[i].valid = 0;
}


memory.c
#include "tlb.h"

in getPhysicalAddress()
change:
int frame = tlb_lookup(proc_id, index);

if (frame == -1) {

    frame = get_page_table_entry(proc_id, index);

    if (frame == 0) {
        printf("Error: page fault\n");
        exit(1);
    }

    tlb_insert(proc_id, index, frame);
}


cache.h
#ifndef CACHE_H
#define CACHE_H

#define CACHE_SIZE 8

void cache_init(void);
int cache_read(int address, int *value);
void cache_write(int address, int value);
void cache_flush(void);

#endif


cache.c
#include "cache.h"

typedef struct {
    int valid;
    int tag;
    int value;
} CacheLine;

static CacheLine cache[CACHE_SIZE];

void cache_init(void)
{
    for (int i = 0; i < CACHE_SIZE; i++)
        cache[i].valid = 0;
}

int cache_read(int address, int *value)
{
    int index = address % CACHE_SIZE;
    int tag = address / CACHE_SIZE;

    if (cache[index].valid &&
        cache[index].tag == tag) {

        *value = cache[index].value;
        return 1;
    }

    return 0;
}

void cache_write(int address, int value)
{
    int index = address % CACHE_SIZE;
    int tag = address / CACHE_SIZE;

    cache[index].valid = 1;
    cache[index].tag = tag;
    cache[index].value = value;
}

void cache_flush(void)
{
    for (int i = 0; i < CACHE_SIZE; i++)
        cache[i].valid = 0;
}



timing.h
#ifndef TIMING_H
#define TIMING_H

extern unsigned long long total_cycles;

void timing_reset(void);
void timing_add(unsigned long long cycles);

#endif


timing.c
#include "timing.h"

unsigned long long total_cycles = 0;

void timing_reset(void)
{
    total_cycles = 0;
}

void timing_add(unsigned long long cycles)
{
    total_cycles += cycles;
}


processor.c
#include "timing.h"

before execute():
timing_add(1);

**case OP_MUL:
    timing_add(3);
    Register[proc_id][dest] =
        Register[proc_id][src1] *
        Register[proc_id][src2];
    break;


--------------------------------------------------------------Extension 7
Processor Pipelining
pipeline.h
#ifndef PIPELINE_H
#define PIPELINE_H

typedef struct {
    int valid;

    int opcode;
    int dest;
    int src1;
    int src2;

    int pc;
} PipelineRegister;

void pipeline_init(void);
void pipeline_step(int proc_id);

#endif



pipeline.c
#include "pipeline.h"

static PipelineRegister IF_ID;
static PipelineRegister ID_EX;

void pipeline_init(void)
{
    IF_ID.valid = 0;
    ID_EX.valid = 0;
}



change
fetch(proc_id);
decode(proc_id);
execute(proc_id);

to
pipeline_step(proc_id);


void pipeline_step(int proc_id)
{
    /*
     * Execute previous instruction
     */
    if (ID_EX.valid) {
        opcode = ID_EX.opcode;
        dest   = ID_EX.dest;
        src1   = ID_EX.src1;
        src2   = ID_EX.src2;

        execute(proc_id);
    }

    /*
     * Move IF/ID into ID/EX
     */
    ID_EX = IF_ID;

    /*
     * Fetch next instruction
     */
    fetch(proc_id);

    IF_ID.valid = 1;
    IF_ID.opcode = opcode;
    IF_ID.dest = dest;
    IF_ID.src1 = src1;
    IF_ID.src2 = src2;
}



------------------------------------------------------------Extension 8
Inter-Process Communication Using Shared Memory
shared_memory.h
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#define MAX_SHARED_SEGMENTS 16

int shm_create(int owner_pid, int size);
int shm_attach(int pid, int shm_id);
void shm_detach(int pid, int shm_id);
void shm_destroy(int shm_id);

#endif


shared_memory.c
#include <stdio.h>
#include "shared_memory.h"

typedef struct {
    int valid;
    int id;
    int owner_pid;
    int start_address;
    int size;
    int reference_count;
} SharedMemory;

static SharedMemory segments[MAX_SHARED_SEGMENTS];

int shm_create(int owner_pid, int size)
{
    for (int i = 0; i < MAX_SHARED_SEGMENTS; i++) {

        if (!segments[i].valid) {

            segments[i].valid = 1;
            segments[i].id = i;
            segments[i].owner_pid = owner_pid;
            segments[i].start_address = 0;
            segments[i].size = size;
            segments[i].reference_count = 1;

            return i;
        }
    }

    return -1;
}

int shm_attach(int pid, int shm_id)
{
    if (shm_id < 0 ||
        shm_id >= MAX_SHARED_SEGMENTS)
        return -1;

    if (!segments[shm_id].valid)
        return -1;

    segments[shm_id].reference_count++;

    return segments[shm_id].start_address;
}

void shm_detach(int pid, int shm_id)
{
    if (shm_id < 0 ||
        shm_id >= MAX_SHARED_SEGMENTS)
        return;

    if (!segments[shm_id].valid)
        return;

    if (segments[shm_id].reference_count > 0)
        segments[shm_id].reference_count--;
}


In os:
#include "shared_memory.h"



----------------------------------------------------------Extension 9
Page Table Security and Access Permissions
change:
unsigned char pageTable[][];

to
typedef struct {
    unsigned char frame;

    unsigned char present;
    unsigned char read;
    unsigned char write;
    unsigned char execute;
} PageTableEntry;


add in memory.h:
typedef struct {
    unsigned char frame;
    unsigned char present;
    unsigned char read;
    unsigned char write;
    unsigned char execute;
} PageTableEntry;


instruction page:
entry.frame = frame;
entry.present = 1;
entry.read = 1;
entry.write = 0;
entry.execute = 1;

data page:
entry.frame = frame;
entry.present = 1;
entry.read = 1;
entry.write = 1;
entry.execute = 0;


getPhysicalAddress():
if (isFetch && !entry.execute) {
    printf("Protection fault: execute permission denied\n");
    exit(1);
}
for data reads:
if (!isFetch && !entry.read) {
    printf("Protection fault: read permission denied\n");
    exit(1);
}


Modify write_word():
if (!entry.write) {
    printf("Protection fault: write permission denied\n");
    exit(1);
}




------------------------------------------------------------Extension 10
Virtual Registers → Physical Registers
liveness.h
#ifndef LIVENESS_H
#define LIVENESS_H

void perform_liveness_analysis(void);

#endif


liveness.c
#include <stdio.h>
#include "liveness.h"

void perform_liveness_analysis(void)
{
    printf("Performing liveness analysis...\n");

    /*
     * For every instruction:
     *
     * USE  = registers read
     * DEF  = registers written
     *
     * LIVE_OUT = union of successor LIVE_IN sets
     *
     * LIVE_IN = USE union
     *           (LIVE_OUT - DEF)
     */
}

interference.h
#ifndef INTERFERENCE_H
#define INTERFERENCE_H

void build_interference_graph(void);

#endif

interference.c
#include <stdio.h>
#include "interference.h"

void build_interference_graph(void)
{
    printf("Building interference graph...\n");

    /*
     * If two virtual registers
     * are simultaneously live,
     * add an edge between them.
     */
}



register_alloc.h
#ifndef REGISTER_ALLOC_H
#define REGISTER_ALLOC_H

#define PHYSICAL_REG_COUNT 8

void allocate_registers(void);

#endif


register_alloc.c
#include <stdio.h>
#include "register_alloc.h"

void allocate_registers(void)
{
    printf("Performing register allocation...\n");

    /*
     * Graph coloring:
     *
     * color = physical register
     *
     * If no color is available,
     * spill the virtual register.
     */
}

compiler
#define SPILL_BASE 1000

compiler.h
#define SPILL_BASE 1000
#define WORD_SIZE 4
#define MAX_SPILLS 32

compiler.c
typedef struct {
    int virtual_register;
    int memory_address;
    int valid;
} SpillSlot;
static SpillSlot spill_slots[MAX_SPILLS];
static int spill_count = 0;

int allocate_spill_slot(int virtual_register)
{
    for (int i = 0; i < spill_count; i++) {

        if (spill_slots[i].virtual_register ==
            virtual_register) {

            return spill_slots[i].memory_address;
        }
    }

    if (spill_count >= MAX_SPILLS) {
        printf("Error: spill area full\n");
        exit(1);
    }

    int address =
        SPILL_BASE +
        spill_count * WORD_SIZE;

    spill_slots[spill_count].virtual_register =
        virtual_register;

    spill_slots[spill_count].memory_address =
        address;

    spill_slots[spill_count].valid = 1;

    spill_count++;

    return address;
}

void generate_spill_store(int physical_register,
                          int virtual_register)
{
    int address =
        allocate_spill_slot(virtual_register);

    printf("STORE R%d, [%d]\n",
           physical_register,
           address);
}

void generate_spill_load(int virtual_register,
                         int physical_register)
{
    int address =
        allocate_spill_slot(virtual_register);

    printf("LOAD [%d], R%d\n",
           address,
           physical_register);
}

compiler.c
#define MAX_VIRTUAL_REGISTERS 128

static int spilled[MAX_VIRTUAL_REGISTERS];
static int physical_register[MAX_VIRTUAL_REGISTERS];

for (int i = 0;
     i < MAX_VIRTUAL_REGISTERS;
     i++) {

    spilled[i] = 0;
    physical_register[i] = -1;
}

if (no_register_available) {

    spilled[vreg] = 1;

    allocate_spill_slot(vreg);
}






Makefile
Current:
$(TARGET): main.c compiler.c processor.c memory.c os.c compiler.h processor.h memory.h os.h
	$(CC) $(CFLAGS) main.c compiler.c processor.c memory.c os.c -o $(TARGET)

4:
$(TARGET): main.c compiler.c processor.c memory.c os.c io.c
	$(CC) $(CFLAGS) main.c compiler.c processor.c memory.c os.c io.c -o $(TARGET)

5:
$(TARGET): main.c compiler.c processor.c memory.c os.c utilities.c
	$(CC) $(CFLAGS) main.c compiler.c processor.c memory.c os.c utilities.c -o $(TARGET)

6:
$(TARGET): main.c compiler.c processor.c memory.c os.c tlb.c cache.c timing.c
	$(CC) $(CFLAGS) main.c compiler.c processor.c memory.c os.c tlb.c cache.c timing.c -o $(TARGET)

7:
$(TARGET): main.c compiler.c processor.c memory.c os.c pipeline.c
	$(CC) $(CFLAGS) main.c compiler.c processor.c memory.c os.c pipeline.c -o $(TARGET)

8:
$(TARGET): main.c compiler.c processor.c memory.c os.c shared_memory.c
	$(CC) $(CFLAGS) main.c compiler.c processor.c memory.c os.c shared_memory.c -o $(TARGET)

10:
$(TARGET): main.c compiler.c processor.c memory.c os.c liveness.c interference.c register_alloc.c
	$(CC) $(CFLAGS) main.c compiler.c processor.c memory.c os.c liveness.c interference.c register_alloc.c -o $(TARGET)