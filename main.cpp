#include "cpu.h"
#include "memory.h"
#include<iostream>

int main()
{
    Memory memory;
    CPU cpu(&memory);

    memory.write(0x0000, 0x3E); // MVI A
    memory.write(0x0001, 0xFF);

    memory.write(0x0002, 0x06); // MVI B
    memory.write(0x0003, 0x01);

    memory.write(0x0004, 0x80); // ADD B
    memory.write(0x0005, 0x76); // HLT

    cpu.run();

    return 0;
}
