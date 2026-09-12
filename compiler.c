#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

#define MAX_LINES   256
#define MAX_LINE    300
#define MAX_LABELS  64

static char lines[MAX_LINES][MAX_LINE];
static int  line_count;

static char label_name[MAX_LABELS][64];
static int  label_index[MAX_LABELS];
static int  label_count;

/* branch suffix table, taken straight from the condition flag table in the lab sheet */
static const char *suffix_table[] = {
    "EQ", "NE", "CS", "CC", "MI", "PL", "VS", "VC",
    "HI", "LS", "GE", "LT", "GT", "LE", "AL"
};
static const int suffix_code[] = {
    BR_EQ, BR_NE, BR_CS, BR_CC, BR_MI, BR_PL, BR_VS, BR_VC,
    BR_HI, BR_LS, BR_GE, BR_LT, BR_GT, BR_LE, BR_AL
};

/*
 * load_program()
 * Reads source_path into the lines[] array, stripping the trailing
 * newline and any '%' comment on each line.
 */
static void load_program(const char *source_path)
{
    FILE *in = fopen(source_path, "r");
    if (in == NULL) {
        printf("Error: could not open %s\n", source_path);
        exit(1);
    }

    line_count = 0;
    char raw[MAX_LINE];

    while (line_count < MAX_LINES && fgets(raw, sizeof(raw), in) != NULL) {
        raw[strcspn(raw, "\r\n")] = '\0';

        /* cut off a trailing comment */
        char *comment = strchr(raw, '%');
        if (comment != NULL) *comment = '\0';

        /* a legacy Read/Write line uses a comma, turn it into a space
           so the rest of the compiler only has to deal with spaces */
        for (char *p = raw; *p != '\0'; p++) {
            if (*p == ',') *p = ' ';
        }

        strncpy(lines[line_count], raw, MAX_LINE - 1);
        lines[line_count][MAX_LINE - 1] = '\0';
        line_count++;
    }

    fclose(in);
}

static int is_blank(const char *line)
{
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isspace((unsigned char)line[i])) return 0;
    }
    return 1;
}

/* a label is the very first character of the line being '.', per the sheet */
static int is_label(const char *line)
{
    return line[0] == '.';
}

static void add_label(const char *line, int instr_index)
{
    if (label_count >= MAX_LABELS) {
        printf("Error: too many labels\n");
        exit(1);
    }
    sscanf(line + 1, "%63[a-zA-Z0-9]", label_name[label_count]);
    label_index[label_count] = instr_index;
    label_count++;
}

static int find_label(const char *name)
{
    for (int i = 0; i < label_count; i++) {
        if (strcmp(label_name[i], name) == 0) return label_index[i];
    }
    printf("Error: undefined label .%s\n", name);
    exit(1);
}

/*
 * pass_one()
 * Walks the program once just to record where every label points to,
 * counting one instruction slot for every non-blank, non-label line.
 */
static void pass_one(void)
{
    label_count = 0;
    int instr_index = 0;

    for (int i = 0; i < line_count; i++) {
        if (is_blank(lines[i])) continue;
        if (is_label(lines[i])) {
            add_label(lines[i], instr_index);
            continue;
        }
        instr_index++;
    }
}

/* parses "B<suffix> .<label>", returns 1 and fills opcode/src2 (offset) on success */
static int parse_branch(const char *line, int instr_index, int *opcode, int *dest, int *src1, int *src2)
{
    if (line[0] != 'B' || !isupper((unsigned char)line[1]) || !isupper((unsigned char)line[2])) {
        return 0;
    }

    char suffix[3] = { line[1], line[2], '\0' };
    int code = -1;
    for (int i = 0; i < 15; i++) {
        if (strcmp(suffix, suffix_table[i]) == 0) { code = suffix_code[i]; break; }
    }
    if (code == -1) return 0;

    const char *p = line + 3;
    while (*p == ' ' || *p == '\t') p++;
    if (*p != '.') return 0;

    char label[64];
    if (sscanf(p + 1, "%63[a-zA-Z0-9]", label) != 1) return 0;

    int target = find_label(label);
    int offset = target - instr_index;

    *opcode = OP_BRANCH_BASE + code;
    *dest = 0;
    *src1 = 0;
    *src2 = (offset < 0) ? offset + 256 : offset;
    return 1;
}

/*
 * parse_instruction()
 * Tries every supported instruction pattern in turn and fills in
 * opcode/dest/src1/src2 for whichever one matches. Returns 1 on a match,
 * 0 if the line was not recognised.
 */
static int parse_instruction(const char *line, int instr_index, int *opcode, int *dest, int *src1, int *src2)
{
    int a, b, c, n;

    if (parse_branch(line, instr_index, opcode, dest, src1, src2)) return 1;

    /* ---- print, dest and operand 1 are both 0, the register goes in operand 2 ---- */
    if (sscanf(line, "Print x%d %n", &a, &n) == 1 && line[n] == '\0') {
        *opcode = OP_PRINT; *dest = 0; *src1 = 0; *src2 = a; return 1;
    }
    if (sscanf(line, "print x%d %n", &a, &n) == 1 && line[n] == '\0') {
        *opcode = OP_PRINT; *dest = 0; *src1 = 0; *src2 = a; return 1;
    }

    /* ---- vector memory ---- */
    if (sscanf(line, "v%d = [ x%d ] %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_VMEMREAD; *dest = a; *src1 = 0; *src2 = b; return 1;
    }
    if (sscanf(line, "v%d = [ %d ] %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_VMEMREAD_C; *dest = a; *src1 = 0; *src2 = b; return 1;
    }
    if (sscanf(line, "[ x%d ] = v%d %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_VMEMWRITE; *dest = a; *src1 = 0; *src2 = b; return 1;
    }
    if (sscanf(line, "[ %d ] = v%d %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_VMEMWRITE_C; *dest = b; *src1 = 0; *src2 = a; return 1;
    }

    /* ---- vector arithmetic: vector-vector ---- */
    if (sscanf(line, "v%d = v%d + v%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_VADD; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "v%d = v%d - v%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_VSUB; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "v%d = v%d * v%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_VMUL; *dest = a; *src1 = b; *src2 = c; return 1;
    }

    /* ---- vector arithmetic: vector-scalar (x register) ---- */
    if (sscanf(line, "v%d = v%d + x%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_VADD_X; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "v%d = v%d - x%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_VSUB_X; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "v%d = v%d * x%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_VMUL_X; *dest = a; *src1 = b; *src2 = c; return 1;
    }

    /* ---- vector arithmetic: vector-constant ---- */
    if (sscanf(line, "v%d = v%d + %d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_VADD_C; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "v%d = v%d - %d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_VSUB_C; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "v%d = v%d * %d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_VMUL_C; *dest = a; *src1 = b; *src2 = c; return 1;
    }

    /* ---- integer memory, bracket form ---- */
    if (sscanf(line, "x%d = [ x%d ] %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_MEMREAD; *dest = a; *src1 = 0; *src2 = b; return 1;
    }
    if (sscanf(line, "x%d = [ %d ] %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_MEMREAD_C; *dest = a; *src1 = 0; *src2 = b; return 1;
    }
    if (sscanf(line, "[ x%d ] = x%d %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_MEMWRITE; *dest = a; *src1 = 0; *src2 = b; return 1;
    }
    if (sscanf(line, "[ %d ] = x%d %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_MEMWRITE_C; *dest = b; *src1 = 0; *src2 = a; return 1;
    }

    /* ---- legacy Read / Write (kept working, not meant to be used from lab 2 onwards) ---- */
    if (sscanf(line, "Read x%d %d %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_MEMREAD_C; *dest = a; *src1 = 0; *src2 = b; return 1;
    }
    if (sscanf(line, "Write x%d %d %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_MEMWRITE_C; *dest = a; *src1 = 0; *src2 = b; return 1;
    }

    /* ---- integer arithmetic: register operand ---- */
    if (sscanf(line, "x%d = x%d + x%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_ADD; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "x%d = x%d - x%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_SUB; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "x%d = x%d * x%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_MUL; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "x%d = x%d / x%d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_DIV; *dest = a; *src1 = b; *src2 = c; return 1;
    }

    /* ---- integer arithmetic: constant operand ---- */
    if (sscanf(line, "x%d = x%d + %d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_ADD_C; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "x%d = x%d - %d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_SUB_C; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "x%d = x%d * %d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_MUL_C; *dest = a; *src1 = b; *src2 = c; return 1;
    }
    if (sscanf(line, "x%d = x%d / %d %n", &a, &b, &c, &n) == 3 && line[n] == '\0') {
        *opcode = OP_DIV_C; *dest = a; *src1 = b; *src2 = c; return 1;
    }

    /* ---- plain register copy ---- */
    if (sscanf(line, "x%d = x%d %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_MOVE; *dest = a; *src1 = 0; *src2 = b; return 1;
    }

    /* ---- data movement ---- */
    if (sscanf(line, "x%d = %d %n", &a, &b, &n) == 2 && line[n] == '\0') {
        *opcode = OP_MOVE_C; *dest = a; *src1 = 0; *src2 = b; return 1;
    }

    return 0;
}

int compile(const char *source_path, const char *bytecode_path)
{
    load_program(source_path);
    pass_one();

    FILE *out = fopen(bytecode_path, "w");
    if (out == NULL) {
        printf("Error: could not create %s\n", bytecode_path);
        exit(1);
    }

    int instr_index = 0;
    int valid_count = 0;

    for (int i = 0; i < line_count; i++) {
        char *line = lines[i];

        if (is_blank(line)) continue;
        if (is_label(line)) continue;

        int opcode, dest, src1, src2;

        if (parse_instruction(line, instr_index, &opcode, &dest, &src1, &src2)) {
            fprintf(out, "%X %X %X %X\n", opcode, dest, src1, src2);
            valid_count++;
        } else {
            printf("Warning: unrecognized line skipped: %s\n", line);
        }

        instr_index++;
    }

    /* stopping condition for the processor */
    fprintf(out, "0 0 0 0\n");

    fclose(out);

    return valid_count;
}
