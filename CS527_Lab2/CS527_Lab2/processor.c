#include "processor.h"
#include "memory.h"

int Register[256];
int PC;
int Z = 0, N = 0, C = 0, V = 0;
int end_of_simulation = 0;

/* fields filled in by fetch(), used by execute() */
static int opcode, dest, src1, src2;
static int cur_addr;   /* address of the instruction currently being executed */

void reset(void)
{
    int i;
    for (i = 0; i < 256; i++)
        Register[i] = 0;
    PC = 0;
    Z = N = C = V = 0;
    end_of_simulation = 0;
}

void fetch(void)
{
    cur_addr = PC;
    opcode = (unsigned char) Instruction[PC];
    dest   = (unsigned char) Instruction[PC + 1];
    src1   = (unsigned char) Instruction[PC + 2];
    src2   = (unsigned char) Instruction[PC + 3];
    PC = PC + 4;
}

void decode(void)
{
    /* nothing to do at this time, kept as per spec */
}

static void set_add_flags(int a, int b, int r)
{
    unsigned int ua = (unsigned int) a;
    unsigned int ub = (unsigned int) b;
    unsigned int ur = (unsigned int) r;
    int sa = (a < 0);
    int sb = (b < 0);
    int sr = (r < 0);

    Z = (r == 0) ? 1 : 0;
    N = (ur & 0x80000000u) ? 1 : 0;
    C = (ur < ua || ur < ub) ? 1 : 0;
    V = (sa == sb && sr != sa) ? 1 : 0;
}

static void set_sub_flags(int a, int b, int r)
{
    unsigned int ur = (unsigned int) r;
    int sa = (a < 0);
    int sb = (b < 0);
    int sr = (r < 0);

    Z = (r == 0) ? 1 : 0;
    N = (ur & 0x80000000u) ? 1 : 0;
    C = ((unsigned int) a > (unsigned int) b) ? 1 : 0;
    V = (sa != sb && sr == sb) ? 1 : 0;
}

/* cond is the low nibble of the branch opcode (opcode - 0x10), matches
   the branch suffix table in the assignment */
static int branch_taken(int cond)
{
    switch (cond) {
        case 0:  return Z == 1;            /* EQ */
        case 1:  return Z == 0;            /* NE */
        case 2:  return C == 1;            /* CS */
        case 3:  return C == 0;            /* CC */
        case 4:  return N == 1;            /* MI */
        case 5:  return N == 0;            /* PL */
        case 6:  return V == 1;            /* VS */
        case 7:  return V == 0;            /* VC */
        case 8:  return C == 1 && Z == 0;  /* HI */
        case 9:  return C == 0 || Z == 1;  /* LS */
        case 10: return N == V;            /* GE */
        case 11: return N != V;            /* LT */
        case 12: return Z == 0 && N == V;  /* GT */
        case 13: return Z == 1 || N != V;  /* LE */
        case 14: return 1;                 /* AL */
        default: return 0;
    }
}

void execute(void)
{
    int a, b, r, addr, val;

    if (opcode == 0) {
        end_of_simulation = 1;
        return;
    }

    if (opcode >= 0x10 && opcode <= 0x1E) {
        int cond = opcode - 0x10;
        int offset = (signed char) src2;  /* two's complement, in units of instructions */
        if (branch_taken(cond))
            PC = cur_addr + offset * 4;
        return;
    }

    switch (opcode) {
        case 0x01: case 0x09:  /* add */
            a = Register[src1];
            b = (opcode == 0x01) ? Register[src2] : src2;
            r = a + b;
            Register[dest] = r;
            set_add_flags(a, b, r);
            break;

        case 0x02: case 0x0A:  /* subtract */
            a = Register[src1];
            b = (opcode == 0x02) ? Register[src2] : src2;
            r = a - b;
            Register[dest] = r;
            set_sub_flags(a, b, r);
            break;

        case 0x03: case 0x0B:  /* multiply */
            a = Register[src1];
            b = (opcode == 0x03) ? Register[src2] : src2;
            Register[dest] = a * b;
            break;

        case 0x04: case 0x0C:  /* divide */
            a = Register[src1];
            b = (opcode == 0x04) ? Register[src2] : src2;
            if (b != 0)
                Register[dest] = a / b;
            break;

        case 0x05:  /* memory read, address in a variable */
            addr = Register[src2];
            Register[dest] = mem_read32(addr);
            break;

        case 0x0D:  /* memory read, constant address */
            addr = src2;
            Register[dest] = mem_read32(addr);
            break;

        case 0x06:  /* memory write, address in a variable (dest holds address reg) */
            addr = Register[dest];
            val = Register[src2];
            mem_write32(addr, val);
            break;

        case 0x0E:  /* memory write, constant address (dest holds the constant) */
            addr = dest;
            val = Register[src2];
            mem_write32(addr, val);
            break;

        case 0x0F:  /* data movement, dest = constant */
            Register[dest] = src2;
            break;

        default:
            break;
    }
}
