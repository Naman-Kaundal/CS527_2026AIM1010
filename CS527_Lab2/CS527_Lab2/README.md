# CS527 Lab 2 - Mini Computer Simulator

## Build

```
make
```

This produces a single executable `sim`.

## Run

```
./sim <program_file>
```

This compiles `<program_file>` into `program.byte` (in the current directory),
then initialises memory from `program.byte` and `data.byte` (if `data.byte`
does not exist, data memory just starts at all zero), executes the program,
and finally writes the ending state of data memory back to `data.byte`.

## Files

- `main.c` - ties compiler, memory and processor together (`compile()` ->
  `initialize()` -> `reset()` -> fetch/decode/execute loop -> `finalize()`)
- `compiler.c` / `compiler.h` - two pass compiler for the language described
  in the assignment. Pass 1 records label positions, pass 2 emits the byte
  code for every instruction line to `program.byte`.
- `processor.c` / `processor.h` - registers, PC, flags (Z N C V) and the
  fetch/decode/execute functions.
- `memory.c` / `memory.h` - `Instruction[256]`, `Data[4096]` byte arrays,
  loading/saving `program.byte` / `data.byte`, and 32 bit little endian
  read/write helpers used by the processor for memory instructions.
- `tests/` - a few sample programs together with the `data.byte` they need.

## Notes / assumptions taken while implementing the spec

1. **`while(end_of_simulation)` in the main() pseudocode.** As written this
   loop would never run at all, since `end_of_simulation` starts at 0. This
   is implemented as `while (!end_of_simulation)`, which is clearly the
   intended behaviour (the loop is meant to run until execute() sees opcode
   0 and sets the flag).

2. **Memory read opcode when the address is a constant.** The opcode table
   lists `0x0C` for both "Divide, constant operand" and "Memory read,
   constant operand", which is a copy/paste typo (every other row follows
   the pattern `var_opcode + 8 = const_opcode`, e.g. Add 0x01/0x09, Sub
   0x02/0x0A, Mul 0x03/0x0B, Div 0x04/0x0C, Write 0x06/0x0E, Data move
   0x07/0x0F). Following that pattern, "Memory read, constant address" is
   implemented as **`0x0D`** instead of the `0x0C` printed in the table, so
   it does not collide with Divide's constant opcode. This only affects the
   `x1 = [<constant>]` / legacy `Read x1, <constant>` forms; nothing in the
   worked example in the assignment exercises this opcode, so it can't
   conflict with the given expected bytecode.

3. **Field layout for memory read/write.** The assignment states "operand 1
   is 0" for read/write/data-move but does not spell out how the address
   and value are split between `dest` and `operand2`. This was reverse
   engineered from the worked example (`x4 = [x3]` -> `5 4 0 3`,
   `[x3] = x2` -> `6 3 0 2`):
   - **Read**: `dest` = destination register, `operand2` = address
     (register number if the opcode is the "variable" variant, or the
     literal address if it's the "constant" variant).
   - **Write**: `dest` = address (register number or literal, same rule as
     above), `operand2` = the register holding the value being written.
   - **Data movement**: `dest` = destination register, `operand2` = the
     constant value.

   With this layout the compiler reproduces the sample program's bytecode
   in the assignment exactly, byte for byte (see `tests/test1_array_sum.txt`).

4. **Branch offset.** The offset byte is in units of *instructions*, not
   bytes (matches the worked example: `BEQ .exit` two instructions before
   `.exit`'s eventual position becomes `6`, and `BAL .loopback` becomes
   `0xFA` = -6). `fetch()` records the byte address of the instruction being
   executed; a taken branch sets `PC = current_instruction_address +
   offset * 4`.

5. **Data size.** Registers and memory operands are treated as 32 bit
   values (as stated at the end of the spec: "Data size is always 32
   bit"), stored little endian in the 4096 byte data memory.

## Testing

`tests/test1_array_sum.txt` is the exact example program from the
assignment; compiling it reproduces the exact bytecode given in the
assignment. Running it with `tests/data_array_sum.byte` as `data.byte`
(array `[1,2,3,4,5]`) produces a sum of 15 in data memory, confirming the
compiler, labels, branches and flags all work end to end.

`tests/test2_legacy_and_arith.txt` exercises the legacy `Read`/`Write`
syntax together with multiply/divide.

`tests/test3_branches.txt` exercises a conditional branch (`BGT`) driven by
the flags set from a subtraction that produces a negative result.

To run a test, e.g.:
```
cd tests
cp data_array_sum.byte data.byte
../sim test1_array_sum.txt
cat data.byte
```
