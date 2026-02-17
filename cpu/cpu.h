#ifndef CPU_H
#define CPU_H

#include <cstdint>

class Memory;

class CPU
{
private:
    Memory* memory;
public:
    CPU(Memory* memory);

    // 8-bit registers
    uint8_t A, B, C, D, E, H, L;

    // 16-bit registers
    uint16_t PC;
    uint16_t SP;

    //Flags
    struct Flags {
        bool S;   // Sign
        bool Z;   // Zero
        bool AC;  // Auxiliary Carry
        bool P;   // Parity
        bool CY;  // Carry
    } flags;

    void execute();
};

#endif // CPU_H
