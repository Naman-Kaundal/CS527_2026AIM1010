#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

#define MAX_INSTR  256
#define MAX_LABELS 256
#define MAX_TOKENS 10

typedef struct {
    char name[32];
    int index;      /* instruction index the label points to */
} Label;

/* lines of the source that turn into an actual instruction (labels do not) */
static char instr_lines[MAX_INSTR][256];
static int instr_count = 0;

static Label labels[MAX_LABELS];
static int label_count = 0;

/* removes trailing \n, \r and leading/trailing spaces/tabs from a line */
static void trim(char *s)
{
    int len = strlen(s);
    int start = 0;

    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r' ||
                        s[len - 1] == ' '  || s[len - 1] == '\t')) {
        s[len - 1] = 0;
        len--;
    }
    while (s[start] == ' ' || s[start] == '\t')
        start++;
    if (start > 0)
        memmove(s, s + start, strlen(s + start) + 1);
}

/* anything after % is a comment */
static void strip_comment(char *s)
{
    char *p = strchr(s, '%');
    if (p != NULL)
        *p = 0;
}

static int find_label(char *name)
{
    char clean[32];
    int i;

    if (name[0] == '.')
        strcpy(clean, name + 1);
    else
        strcpy(clean, name);

    for (i = 0; i < label_count; i++) {
        if (strcmp(labels[i].name, clean) == 0)
            return labels[i].index;
    }

    fprintf(stderr, "compile error: undefined label %s\n", name);
    exit(1);
}

/* tok is something like "x12", returns 12 */
static int parse_reg(char *tok)
{
    return atoi(tok + 1);
}

static int suffix_code(char *s)
{
    if (strcmp(s, "EQ") == 0) return 0;
    if (strcmp(s, "NE") == 0) return 1;
    if (strcmp(s, "CS") == 0) return 2;
    if (strcmp(s, "CC") == 0) return 3;
    if (strcmp(s, "MI") == 0) return 4;
    if (strcmp(s, "PL") == 0) return 5;
    if (strcmp(s, "VS") == 0) return 6;
    if (strcmp(s, "VC") == 0) return 7;
    if (strcmp(s, "HI") == 0) return 8;
    if (strcmp(s, "LS") == 0) return 9;
    if (strcmp(s, "GE") == 0) return 10;
    if (strcmp(s, "LT") == 0) return 11;
    if (strcmp(s, "GT") == 0) return 12;
    if (strcmp(s, "LE") == 0) return 13;
    if (strcmp(s, "AL") == 0) return 14;

    fprintf(stderr, "compile error: unknown branch suffix %s\n", s);
    exit(1);
}

/* pads '[' ']' '=' ',' with spaces so that a simple whitespace based
   tokenizer can split the line into tokens */
static void normalize(char *in, char *out)
{
    int i, j = 0;
    int len = strlen(in);

    for (i = 0; i < len; i++) {
        char c = in[i];
        if (c == '[' || c == ']' || c == '=') {
            out[j++] = ' ';
            out[j++] = c;
            out[j++] = ' ';
        } else if (c == ',') {
            /* comma is just a separator (legacy Read/Write syntax), drop it */
            out[j++] = ' ';
        } else {
            out[j++] = c;
        }
    }
    out[j] = 0;
}

static int tokenize(char *line, char tokens[][32])
{
    int n = 0;
    char *tok = strtok(line, " \t");

    while (tok != NULL && n < MAX_TOKENS) {
        strcpy(tokens[n], tok);
        n++;
        tok = strtok(NULL, " \t");
    }
    return n;
}

/* parses one instruction line (label lines are handled by compile()
   before this is ever called) and fills in the four byte code fields */
static void parse_instr(int idx, char *line, int *opcode, int *dest, int *op1, int *op2)
{
    char norm[512];
    char tokens[MAX_TOKENS][32];
    int n;

    normalize(line, norm);
    n = tokenize(norm, tokens);

    *op1 = 0;

    /* legacy: Read <var> <address> */
    if (strcmp(tokens[0], "Read") == 0) {
        *dest = parse_reg(tokens[1]);
        if (tokens[2][0] == 'x') {
            *opcode = 0x05;
            *op2 = parse_reg(tokens[2]);
        } else {
            *opcode = 0x0D;
            *op2 = atoi(tokens[2]);
        }
        return;
    }

    /* legacy: Write <var> <address> */
    if (strcmp(tokens[0], "Write") == 0) {
        int valreg = parse_reg(tokens[1]);
        if (tokens[2][0] == 'x') {
            *opcode = 0x06;
            *dest = parse_reg(tokens[2]);
        } else {
            *opcode = 0x0E;
            *dest = atoi(tokens[2]);
        }
        *op2 = valreg;
        return;
    }

    /* branch: B<suffix> <label> */
    if (tokens[0][0] == 'B' && strlen(tokens[0]) == 3) {
        int cond = suffix_code(tokens[0] + 1);
        int target = find_label(tokens[1]);
        int offset = target - idx;

        *opcode = 0x10 + cond;
        *dest = 0;
        *op2 = offset & 0xFF;
        return;
    }

    /* memory write: [xN] = xM */
    if (tokens[0][0] == '[') {
        char *addr_tok = tokens[1];
        char *val_tok  = tokens[4];

        *opcode = 0x06;
        *dest = parse_reg(addr_tok);
        *op2 = parse_reg(val_tok);
        return;
    }

    /* everything left is of the form xD = ... */
    *dest = parse_reg(tokens[0]);

    /* memory read: xD = [addr] */
    if (tokens[2][0] == '[') {
        char *addr_tok = tokens[3];
        if (addr_tok[0] == 'x') {
            *opcode = 0x05;
            *op2 = parse_reg(addr_tok);
        } else {
            *opcode = 0x0D;
            *op2 = atoi(addr_tok);
        }
        return;
    }

    /* data movement: xD = constant */
    if (n == 3) {
        *opcode = 0x0F;
        *op2 = atoi(tokens[2]);
        return;
    }

    /* arithmetic: xD = xS1 <op> operand2 */
    {
        int src1 = parse_reg(tokens[2]);
        char op = tokens[3][0];
        char *op2tok = tokens[4];
        int base;

        if (op == '+') base = 0x01;
        else if (op == '-') base = 0x02;
        else if (op == '*') base = 0x03;
        else base = 0x04;

        *op1 = src1;
        if (op2tok[0] == 'x') {
            *opcode = base;
            *op2 = parse_reg(op2tok);
        } else {
            *opcode = base + 8;
            *op2 = atoi(op2tok);
        }
    }
}

void compile(char *filename)
{
    FILE *fp, *out;
    char line[256];
    int i;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "compile error: cannot open %s\n", filename);
        exit(1);
    }

    instr_count = 0;
    label_count = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        strip_comment(line);
        trim(line);

        if (strlen(line) == 0)
            continue;

        if (line[0] == '.') {
            strcpy(labels[label_count].name, line + 1);
            labels[label_count].index = instr_count;
            label_count++;
            continue;
        }

        strcpy(instr_lines[instr_count], line);
        instr_count++;
    }
    fclose(fp);

    out = fopen("program.byte", "w");
    if (out == NULL) {
        fprintf(stderr, "compile error: cannot write program.byte\n");
        exit(1);
    }

    for (i = 0; i < instr_count; i++) {
        int opcode, dest, op1, op2;
        parse_instr(i, instr_lines[i], &opcode, &dest, &op1, &op2);
        fprintf(out, "%X %X %X %X\n", opcode, dest, op1, op2);
    }
    fclose(out);
}
