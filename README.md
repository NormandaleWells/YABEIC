# YABEIC - Yet Another Ben Eater-Inspired Computer

 This repo documents the 8-bit breadboard computer I'm building.
 This computer was inspired by Ben Eater's excellent series of YouTube videos,
 which you can find here: [Building an 8-bit breadboard computer!](https://www.youtube.com/watch?v=HyznrdDSSGM&list=PLowKtXNTBypGqImE405J2565dvjafglHU)
 
Perhaps I should refer to this instead as a /r/beneater inspired project;
it's the folks hanging out there that inspired me to create my own design
(despite being nowhere near ready to do so!).

## So how is this one different?

It seems that each person who creates their own BE-inspired 8-bit machine
has a different set of goals, and their own set of features that others
have not thought to incorporate into their designs.  This section
describes the things I'm doing a bit differently from others.

Reading the architecture document first may make some of the following more understandable.

### Better ALU

I plan to use a pair of 74LS181s for the ALU.  This gives me addition and subtraction along with all the basic logical functions
like AND, OR, XOR, and COM.
As with many designs, the A and B registers feed directly into the ALU,
and the B register is not directly accessible to the programmer.

The fact that this chip is only available as an LS-family chip drives the requirement that HCT (or LS) chips
be used throughout.

Since the '181 has a functions setting that allows it to just output the B register,
this also allows me to use the B register as a temporary register during, say, register swap operations.

### Addressing modes

The architecture is a load/store architecture with 4 addressing modes:
1. indirect - the register holds the address of the operand
2. offset - the register holds a base address, and the instruction specifies an 8-bit offset that is added to that address
3. pre-decrement - the register holds the address of the operand, and is automatically decremented before accessing it
4. post-increment - the register holds the address of the operand, and is automatically incremented after accessing it

Of course, there is also register direct, in which the register holds the operand itself.

### Arduino-based incremental development

I plan to create the computer entirely within an Arduino, and gradually replace pieces with actual breadboard-based hardware.

For example, the first module created will be the A and B registers.  (Done as of this writing.)
All other components will be simulated by an Arduino Mega that will read/write the bus and supply the control signals
(at least until the control logic is built).  One exception is that I'm going to implement the A and B registers
and the ALU without ever simulating them.  After that, I'll probably switch between simulating components on
the Arduino, and then implementing them on a breadboard.

In case it isn't obvious, I haven't thought this through completely as of yet, but my current plan is to develop
the components in this order:

1. A and B registers
2. ALU
3. Simulated program counter, instruction register, and RAM (and MAR)

At this point, I can start to implement the actual instruction set, starting with ALU operations.

4. Simulated general-purpose registers
5. General-purpose registers
6. Simulated address calculator
7. Simulated stack pointer (and subroutine call/return instructions).
8. Program counter, stack pointer, and address calculator.

That last item may be the most difficult part of the build.

9. RAM
10. IR and control logic
11. Clock

To help with this, I've created an interface board that sits between the Arduino and the YABEIC.
This consists of a '245 bus transceiver, and two parallel connections to the Arduino (using ports
F and K).  The 8 bits of port F are connected to the input side of the '245, and are used to write
data to the bus.  The 8 bits of port K are connected to the bus side of the '245, and can be used
to read the bus at any time.  The bus itself will initially be a series of 8-bit parallel connectors
connecting the output ends of various '245s.

For the control lines, the interface has a set of 4 '595 shift registers.  I have written an
Arduino library that lets me efficiently write a 32-bit control word to the '595s in about
100 usecs, by bypassing `digitalWrite()` and doing ANDs and ORs to the actual Arduino port.
(This cut the time down from 660 usecs to 100 usecs.)  I'm hoping to get to 10KHz,
so I still need to speed this up a little more.

BTW, despite accessing the ports directly, the code is portable among different types
of Arduinos.

Ironically, even the clock will probably be the last piece incorporated into the computer,
it was the first piece actually built.  I wanted to play around with Julian Ilett's
clock module, and found it will work fine for my purposes.  Depending on the
capacitor I use, I can get either 1.7KHz (with a 1 uF cap), or about 12KHz (with a 0.1 uF cap).
That should be plenty fast for what I'm doing.

### Orthogonal instruction set

My hope is that by having a very orthogonal instruction set, I can use bits of the instruction register
as control signals.  For example, there are two bits in most instructions that indicate a register
number; rather than having 8 signals (in and out for each of 4 registers) I can have 2 signals,
one for reading and one for writing, with the register number coming from the IR.

Likewise for the flags; there are 2 bits in some instructions which select one of four flags
from the flags register (zero, carry, overflow, negative), which is then used to determine
whether a jump or skip is performed.

### Microcode enhancements

Most designs allow a certain number of micro-ops per instruction.  Ben's design goes through
that many cycles no matter what; others have incorporated a "reset sequence" control bit
that can be used to reset the micro-op sequencer early.

My plan is to have 1K micro-ops total, with 48 bit each.  The last 10 bits indicate the
index of the next micro-op.  There will be some special logic that maps sequence index
0 to micro-op 0, which will be the start of the fetch cycle.  Likewise, sequence
index 2 will always map to something based on the instruction register (perhaps
0x300 + IR).  After that, each micro-op has the address of the next one.

Or something like that.

### Separate address bus and address calculator

Supporting address modes will be more efficient if I have a separate address bus
fed from an address calculator.  The address calculate always adds a register
(R2, R3, PC, or SP) and either 1, -1, 0, or the contents of the data bus (the
last is used for the offset mode).  The result is fed to the MAR and,
optionally, the main data bus (or perhaps a more direct route back to
one of the address registers).

The address calculator will use a pair of '283 adder chips.  Unless I think
of a better way, the second input (0, 1, -1, or the offset) will be
selected by enabling one of 4 '245s, 3 of which are hard-coded to the
constants, and the last of which is fed from the data bus.

### Minimal I/O

This may seem odd, but I'm more interested in the computational side of things
than in I/O.  I'll probably have a single output device (probably just 8 LEDs)
and a single input device (probably DIP switches).
