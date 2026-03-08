#include "cpu.h"
#include "memory.h"
#include <iostream>

/*
---------------------------------------------------------
MAIN PROGRAM

This file acts as a simple test harness for the 8085 CPU
simulator.

Steps performed here:
1. Create Memory module
2. Create CPU and connect it to memory
3. Load a small machine-code program into memory
4. Start CPU execution using cpu.run()

The program below tests:
- MVI instruction
- INR instruction
- DCR instruction
- HLT instruction
---------------------------------------------------------
*/

int main()
{
    // Create memory and CPU objects
    Memory memory;
    CPU cpu(&memory);

    /*
    -----------------------------------------------------
    Program loaded into memory (machine code)

    Address   Instruction
    --------------------------
    0000      MVI B,03
    0002      INR B
    0003      INR B
    0004      DCR B
    0005      HLT

    Expected execution:

    B = 3
    B = B + 1 → 4
    B = B + 1 → 5
    B = B - 1 → 4

    Final result: B = 4
    -----------------------------------------------------
    */

    memory.write(0x0000, 0x06); // MVI B opcode
    memory.write(0x0001, 0x03); // Data = 3

    memory.write(0x0002, 0x04); // INR B
    memory.write(0x0003, 0x04); // INR B again
    memory.write(0x0004, 0x05); // DCR B

    memory.write(0x0005, 0x76); // HLT instruction

    // Start CPU execution
    cpu.run();

    return 0;
}
