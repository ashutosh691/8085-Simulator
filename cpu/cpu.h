#ifndef CPU_H
#define CPU_H

#include <cstdint>

class Memory;

/*
---------------------------------------------------------
CPU CLASS

This class represents the core of the 8085 processor.
It simulates the internal state of the CPU and executes
instructions by reading opcodes from memory.

The CPU interacts with:
1. Registers (A, B, C, D, E, H, L)
2. Flags (Zero, Sign, Parity, Carry, Auxiliary Carry)
3. Program Counter (PC)
4. Stack Pointer (SP)
5. External Memory

The run() function performs the classic
FETCH → DECODE → EXECUTE cycle.
---------------------------------------------------------
*/

class CPU
{
public:

    /* Constructor
       The CPU receives a pointer to the Memory module so
       that it can read instructions and data from memory.
    */
    CPU(Memory* mem);

    /* Main execution loop
       Continuously fetches and executes instructions until
       a HLT instruction is encountered.
    */
    void run();

private:

    /* ----------- REGISTERS ----------- */

    // 8-bit general purpose registers
    uint8_t A;   // Accumulator
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;
    uint8_t H;
    uint8_t L;

    // 16-bit registers
    uint16_t PC;   // Program Counter
    uint16_t SP;   // Stack Pointer

    // CPU halt state
    bool halted;

    // Pointer to main memory
    Memory* memory;

    /* ----------- FLAG REGISTER ----------- */

    struct Flags
    {
        bool S;   // Sign flag
        bool Z;   // Zero flag
        bool AC;  // Auxiliary carry
        bool P;   // Parity
        bool CY;  // Carry
    } flags;

    /* ----------- HELPER FUNCTIONS ----------- */

    // Updates flags after arithmetic operations
    void updateArithmeticFlags(uint16_t result);

    // Calculates parity (true if even number of 1 bits)
    bool calculateParity(uint8_t value);

    // Returns register value based on encoded register code
    uint8_t getRegister(uint8_t code);

    // Writes value to a register based on encoded register code
    void setRegister(uint8_t code, uint8_t value);

    // Reads register OR memory depending on register code
    uint8_t readFromCode(uint8_t code);

    // Writes to register OR memory depending on register code
    void writeFromCode(uint8_t code, uint8_t value);
};

#endif
