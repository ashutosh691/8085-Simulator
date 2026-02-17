#include "cpu.h"
#include "memory.h"
#include <iostream>

CPU::CPU(Memory* mem) : memory(mem)
{
    A = B = C = D = E = H = L = 0;
    PC = 0;
    SP = 0;
    flags = {false, false, false, false, false};
}

void CPU::execute()
{
    uint8_t opcode = memory->read(PC);
    PC++;

    std::cout << "Fetched opcode: " << std::hex << (int)opcode << std::endl;
}
