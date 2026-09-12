#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "processor.h"
#include "compiler.h"

int Register[NP][REG_COUNT];
int VRegister[NP][VREG_COUNT][VLANES];

int PC[NP];
int opcode;
int dest;
int src1;
int src2;
int end_of_simulation[NP];

int flagZ[NP], flagN[NP], flagC[NP], flagV[NP];

FILE *fd_log = NULL;

void reset(int proc_id)
{
    for (int i = 0; i < REG_COUNT; i++) Register[proc_id][i] = 0;
    for (int i = 0; i < VREG_COUNT; i++)
        for (int j = 0; j < VLANES; j++) VRegister[proc_id][i][j] = 0;

    PC[proc_id] = 0;
    end_of_simulation[proc_id] = 0;
    flagZ[proc_id] = flagN[proc_id] = flagC[proc_id] = flagV[proc_id] = 0;

    /* the log is shared by every processor, so it only needs opening once */
    if (fd_log == NULL) {
        fd_log = fopen("os_log.txt", "a");
        if (fd_log == NULL) {
            printf("Error: could not open os_log.txt\n");
            exit(1);
        }
    }
}

void fetch(int proc_id)
{
    int phys = getPhysicalAddress(proc_id, 1, PC[proc_id]);

    opcode = memory[phys];
    dest   = memory[phys + 1];
    src1   = memory[phys + 2];
    src2   = memory[phys + 3];

    PC[proc_id] = PC[proc_id] + 4;
}

void decode(int proc_id)
{
    /* Left empty */
    (void)proc_id;
}

/* offset is stored as a single unsigned byte, convert it back to signed */
static int to_signed_byte(int value)
{
    if (value > 127) return value - 256;
    return value;
}

static void update_flags_add(int proc_id, int a, int b, int result)
{
    unsigned int ua = (unsigned int)a, ub = (unsigned int)b, ur = (unsigned int)result;

    flagZ[proc_id] = (result == 0);
    flagN[proc_id] = (result < 0);
    flagC[proc_id] = (ur < ua) || (ur < ub);
    flagV[proc_id] = ((a >= 0) == (b >= 0)) && ((result >= 0) != (a >= 0));
}

static void update_flags_sub(int proc_id, int a, int b, int result)
{
    flagZ[proc_id] = (result == 0);
    flagN[proc_id] = (result < 0);
    flagC[proc_id] = ((unsigned int)a >= (unsigned int)b);
    flagV[proc_id] = ((a >= 0) != (b >= 0)) && ((result >= 0) == (b >= 0));
}

static int branch_taken(int proc_id, int code)
{
    switch (code) {
        case BR_EQ: return flagZ[proc_id];
        case BR_NE: return !flagZ[proc_id];
        case BR_CS: return flagC[proc_id];
        case BR_CC: return !flagC[proc_id];
        case BR_MI: return flagN[proc_id];
        case BR_PL: return !flagN[proc_id];
        case BR_VS: return flagV[proc_id];
        case BR_VC: return !flagV[proc_id];
        case BR_HI: return flagC[proc_id] && !flagZ[proc_id];
        case BR_LS: return !flagC[proc_id] || flagZ[proc_id];
        case BR_GE: return flagN[proc_id] == flagV[proc_id];
        case BR_LT: return flagN[proc_id] != flagV[proc_id];
        case BR_GT: return !flagZ[proc_id] && (flagN[proc_id] == flagV[proc_id]);
        case BR_LE: return flagZ[proc_id] || (flagN[proc_id] != flagV[proc_id]);
        case BR_AL: return 1;
        default:    return 0;
    }
}

void execute(int proc_id)
{
    int result;

    /* a branch opcode is OP_BRANCH_BASE + condition code */
    if (opcode >= OP_BRANCH_BASE && opcode <= OP_BRANCH_BASE + BR_AL) {
        int code = opcode - OP_BRANCH_BASE;
        if (branch_taken(proc_id, code)) {
            int offset = to_signed_byte(src2);
            PC[proc_id] = (PC[proc_id] - 4) + offset * 4;
        }
        return;
    }

    switch (opcode) {
        case OP_END:
            end_of_simulation[proc_id] = 1;
            break;

        case OP_PRINT:
            fprintf(fd_log, "Process id: %d   x%d : %X\n", proc_id, src2, Register[proc_id][src2]);
            fflush(fd_log);
            break;

        /* ---- scalar integer, register operand ---- */
        case OP_ADD:
            result = Register[proc_id][src1] + Register[proc_id][src2];
            update_flags_add(proc_id, Register[proc_id][src1], Register[proc_id][src2], result);
            Register[proc_id][dest] = result;
            break;
        case OP_SUB:
            result = Register[proc_id][src1] - Register[proc_id][src2];
            update_flags_sub(proc_id, Register[proc_id][src1], Register[proc_id][src2], result);
            Register[proc_id][dest] = result;
            break;
        case OP_MUL:
            Register[proc_id][dest] = Register[proc_id][src1] * Register[proc_id][src2];
            break;
        case OP_DIV:
            if (Register[proc_id][src2] == 0) {
                printf("Error: division by zero at PC=%d (proc %d)\n", PC[proc_id] - 4, proc_id);
                exit(1);
            }
            Register[proc_id][dest] = Register[proc_id][src1] / Register[proc_id][src2];
            break;
        case OP_MEMREAD:
            Register[proc_id][dest] = read_word(proc_id, Register[proc_id][src2]);
            break;
        case OP_MEMWRITE:
            write_word(proc_id, Register[proc_id][dest], Register[proc_id][src2]);
            break;
        case OP_MOVE:
            Register[proc_id][dest] = Register[proc_id][src2];
            break;

        /* ---- scalar integer, constant operand ---- */
        case OP_ADD_C:
            result = Register[proc_id][src1] + src2;
            update_flags_add(proc_id, Register[proc_id][src1], src2, result);
            Register[proc_id][dest] = result;
            break;
        case OP_SUB_C:
            result = Register[proc_id][src1] - src2;
            update_flags_sub(proc_id, Register[proc_id][src1], src2, result);
            Register[proc_id][dest] = result;
            break;
        case OP_MUL_C:
            Register[proc_id][dest] = Register[proc_id][src1] * src2;
            break;
        case OP_DIV_C:
            if (src2 == 0) {
                printf("Error: division by zero at PC=%d (proc %d)\n", PC[proc_id] - 4, proc_id);
                exit(1);
            }
            Register[proc_id][dest] = Register[proc_id][src1] / src2;
            break;
        case OP_MEMREAD_C:
            Register[proc_id][dest] = read_word(proc_id, src2);
            break;
        case OP_MEMWRITE_C:
            write_word(proc_id, src2, Register[proc_id][dest]);
            break;
        case OP_MOVE_C:
            Register[proc_id][dest] = src2;
            break;

        /* ---- vector, register/vector operand ---- */
        case OP_VADD:
            for (int i = 0; i < VLANES; i++)
                VRegister[proc_id][dest][i] = VRegister[proc_id][src1][i] + VRegister[proc_id][src2][i];
            break;
        case OP_VSUB:
            for (int i = 0; i < VLANES; i++)
                VRegister[proc_id][dest][i] = VRegister[proc_id][src1][i] - VRegister[proc_id][src2][i];
            break;
        case OP_VMUL:
            for (int i = 0; i < VLANES; i++)
                VRegister[proc_id][dest][i] = VRegister[proc_id][src1][i] * VRegister[proc_id][src2][i];
            break;
        case OP_VADD_X:
            for (int i = 0; i < VLANES; i++)
                VRegister[proc_id][dest][i] = VRegister[proc_id][src1][i] + Register[proc_id][src2];
            break;
        case OP_VSUB_X:
            for (int i = 0; i < VLANES; i++)
                VRegister[proc_id][dest][i] = VRegister[proc_id][src1][i] - Register[proc_id][src2];
            break;
        case OP_VMUL_X:
            for (int i = 0; i < VLANES; i++)
                VRegister[proc_id][dest][i] = VRegister[proc_id][src1][i] * Register[proc_id][src2];
            break;
        case OP_VMEMREAD: {
            int base = Register[proc_id][src2];
            for (int i = 0; i < VLANES; i++) VRegister[proc_id][dest][i] = read_word(proc_id, base + i * 4);
            break;
        }
        case OP_VMEMWRITE: {
            int base = Register[proc_id][dest];
            for (int i = 0; i < VLANES; i++) write_word(proc_id, base + i * 4, VRegister[proc_id][src2][i]);
            break;
        }

        /* ---- vector, constant operand ---- */
        case OP_VADD_C:
            for (int i = 0; i < VLANES; i++) VRegister[proc_id][dest][i] = VRegister[proc_id][src1][i] + src2;
            break;
        case OP_VSUB_C:
            for (int i = 0; i < VLANES; i++) VRegister[proc_id][dest][i] = VRegister[proc_id][src1][i] - src2;
            break;
        case OP_VMUL_C:
            for (int i = 0; i < VLANES; i++) VRegister[proc_id][dest][i] = VRegister[proc_id][src1][i] * src2;
            break;
        case OP_VMEMREAD_C: {
            int base = src2;
            for (int i = 0; i < VLANES; i++) VRegister[proc_id][dest][i] = read_word(proc_id, base + i * 4);
            break;
        }
        case OP_VMEMWRITE_C: {
            int base = src2;
            for (int i = 0; i < VLANES; i++) write_word(proc_id, base + i * 4, VRegister[proc_id][dest][i]);
            break;
        }

        default:
            printf("Error: unknown opcode %d at PC=%d (proc %d)\n", opcode, PC[proc_id] - 4, proc_id);
            exit(1);
    }
}

void process_instructions(int proc_id, int instruction_count)
{
    for (int i = 0; i < instruction_count && !end_of_simulation[proc_id]; i++) {
        fetch(proc_id);
        decode(proc_id);
        execute(proc_id);
    }

#ifdef _WIN32
    Sleep(1);        /* Windows has no microsecond sleep, 1 ms is close enough */
#else
    usleep(10);
#endif
}
