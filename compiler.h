#ifndef COMPILER_H
#define COMPILER_H

int compile(const char *source_path, const char *bytecode_path);

#define OP_END           0x00

#define OP_ADD           0x01
#define OP_SUB           0x02
#define OP_MUL           0x03
#define OP_DIV           0x04
#define OP_MEMREAD       0x05
#define OP_MEMWRITE      0x06
#define OP_MOVE          0x07
#define OP_PRINT         0x08

#define OP_ADD_C         0x09
#define OP_SUB_C         0x0A
#define OP_MUL_C         0x0B
#define OP_DIV_C         0x0C
#define OP_MEMREAD_C     0x0D   /* moved here, see note above */
#define OP_MEMWRITE_C    0x0E
#define OP_MOVE_C        0x0F

#define OP_BRANCH_BASE   0x10  /* + branch code 0x0-0xE */

#define OP_VADD          0x21
#define OP_VSUB          0x22
#define OP_VMUL          0x23
#define OP_VADD_X        0x24  /* vector op x-register scalar, extra slot */
#define OP_VMEMREAD       0x25
#define OP_VMEMWRITE      0x26
#define OP_VSUB_X         0x27  /* extra slot */
#define OP_VMUL_X         0x28  /* extra slot */
#define OP_VADD_C         0x29
#define OP_VSUB_C         0x2A
#define OP_VMUL_C         0x2B
#define OP_VMEMREAD_C     0x2C
#define OP_VMEMWRITE_C    0x2E

/* branch condition codes, matches the suffix table in the lab sheet */
#define BR_EQ 0x0
#define BR_NE 0x1
#define BR_CS 0x2
#define BR_CC 0x3
#define BR_MI 0x4
#define BR_PL 0x5
#define BR_VS 0x6
#define BR_VC 0x7
#define BR_HI 0x8
#define BR_LS 0x9
#define BR_GE 0xA
#define BR_LT 0xB
#define BR_GT 0xC
#define BR_LE 0xD
#define BR_AL 0xE

#endif
