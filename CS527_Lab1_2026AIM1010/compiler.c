#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

/*
 * trim_newline()
 * Removes trailing '\n' and/or '\r' characters from a line so that string parsing works correctly even for program.txt on Windows.
 */
static void trim_newline(char *line)
{
    line[strcspn(line, "\r\n")] = '\0';
}

/*
 * is_blank()
 * Returns 1 if a line is empty, otherwise 0. Used for compiler to skip blank lines.
 */
static int is_blank(const char *line)
{
    for (int i = 0; line[i] != '\0'; i++) {
        if (!isspace(line[i])) {
            return 0;
        }
    }
    return 1;
}

void compile(void)
{
    FILE *in = fopen("program.txt", "r");
    if (in == NULL) {
        printf("Error: could not open program.txt\n");
        exit(1);
    }

    FILE *out = fopen("program.byte", "w");
    if (out == NULL) {
        printf("Error: could not create program.byte\n");
        fclose(in);
        exit(1);
    }

    char line[300];

    while (fgets(line, sizeof(line), in) != NULL) {
        trim_newline(line);

        if (is_blank(line)) {
            continue; /* Ignores blank lines */
        }

        int opcode = 0, dest = 0, src1 = 0, src2 = 0;
        int matched = 0;

        /* Read x<dest>, <address> , opcode 5 */
        if (!matched && sscanf(line, "Read x%d , %d", &dest, &src1) == 2) {
            opcode = 5;
            src2 = 0;
            matched = 1;
        }

        /* Write x<dest>, <address> , opcode 6 */
        if (!matched && sscanf(line, "Write x%d , %d", &dest, &src1) == 2) {
            opcode = 6;
            src2 = 0;
            matched = 1;
        }

        /* x<dest> = x<src1> + x<src2> , opcode 1 */
        if (!matched && sscanf(line, "x%d = x%d + x%d", &dest, &src1, &src2) == 3) {
            opcode = 1;
            matched = 1;
        }

        /* x<dest> = x<src1> - x<src2> , opcode 2 */
        if (!matched && sscanf(line, "x%d = x%d - x%d", &dest, &src1, &src2) == 3) {
            opcode = 2;
            matched = 1;
        }

        /* x<dest> = x<src1> * x<src2> , opcode 3 */
        if (!matched && sscanf(line, "x%d = x%d * x%d", &dest, &src1, &src2) == 3) {
            opcode = 3;
            matched = 1;
        }

        /* x<dest> = x<src1> / x<src2> , opcode 4 */
        if (!matched && sscanf(line, "x%d = x%d / x%d", &dest, &src1, &src2) == 3) {
            opcode = 4;
            matched = 1;
        }

        /* x<dest> = <constant> , opcode 7 (Move) */
        if (!matched && sscanf(line, "x%d = %d", &dest, &src1) == 2) {
            opcode = 7;
            src2 = 0;
            matched = 1;
        }

        if (matched) {
            fprintf(out, "%d %d %d %d\n", opcode, dest, src1, src2);
        } else {
            printf("Warning: unrecognized line skipped: %s\n", line);
        }
    }

    /* Stopping condition for processor */
    fprintf(out, "0 0 0 0\n");

    fclose(in);
    fclose(out);
}