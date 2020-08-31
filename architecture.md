# Architecture

This page describes the YABEIC architecture.

YABEIC is an 8-bit computer with 8-bit data and 8-bit addresses, using a load/store architecture.
It has an accumulator and 4 general-purpose registers (GPRs).
There is also a stack pointer and a program counter, both of which (along with 2 of the GPRs) may be used in addressing.

## Registers

There are 7 registers visible to the programmer, and several others that operate behind the scenes.

* A register - this is the first operand of all ALU operations, and the destination.  It is fed directly into the ALU (along with the B register).
* B register - the second operand of all ALU operations is loaded here, and fed directly into the ALU.
Any GPR may be transferred to the B register, but the B register is not directly accessible to the programmer.
* R0, R1, R2, R3 - these are the general-purpose registers.  R2 and R3 may be used as address registers.
* SP (stack pointer) - this a pre-decrement, post-increment stack pointer.  It is initialized to 0, so the first push pre-decrements it to 0xFF.
* PC (program counter)
* IR (instruction register)
* MAR (memory address register)

SP and PC may be used as address registers, with some limitations as to the address modes that may be used with them.
For example, using pre-decrement and post-increment modes with the PC is non-sensical,
so the opcodes that would map to those (by nature of the othogonal instruction set) are used for other purposes.

## Load/store and address modes

This is a load/store architecture.  All data must be read into a register before being operated on.
There are no ALU operations that use memory; only a GPR may be used as the second operand.

There are 4 addressing modes used when reading/writing memory:
* indirect - the register contains the address of the operand
* offset - an offset (second instruction byte) is added to the register to get the address of the operand.
* predecrement - the register contains the address of the operand, and is decremented before the access
* postincrement - the register contains the address of the operand, and is incremented after the access

R2 and R3 may be used with all modes.
SP may be used with predecrement only on a store (PUSH), and with postincrement only on a load (POP).
PC may be used only with an offset.
The opcodes that would normally map to invalid SP/PC operations are used for other purposes.

Note that there is no direct memory addressing.  However, the assembler may "support" this using
an offset from the PC.

## ALU operations

The ALU supports the following operations:

* ADD
* ADC (add with carry)
* SUB
* SBB (subtract with borrow)
* AND
* OR
* XOR
* RSUB (reverse subtract) (note 1, note 2)
* NOT (accumulator only) (note 1)
* INC (accumulator only) (note 1)
* DEC (accumulator only) (note 1)
* CLR (accumulator only) (note 1)
* NEG (accumulator only) (note 1)
* LD0 (load 0 - A or GPR) (note 1)
* LD1 (load 1 - A or GPR) (note 1)

Note 1: This instruction is not currently in the architecture, but I think I can squeeze it in.
Note 2: As I wrote this, it occurred to me that this can be supported by moving A to B, move Rn to A, and subtracting.

## Instruction set

