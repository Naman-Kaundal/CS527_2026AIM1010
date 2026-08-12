#ifndef COMPILER_H
#define COMPILER_H

/*
 * compile()
 * Reads program line by line from program.txt, recognizes each supported instruction type, converts it into numeric bytecode (opcode dest src1 src2), and writes the result
 * to program.byte. Blank lines are ignored. A terminating instruction "0 0 0 0" is appended at the end
 */
void compile(void);

#endif