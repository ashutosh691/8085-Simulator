#include "cpu.h"
#include "memory.h"
#include <iostream>

int main()
{
    Memory memory;
    CPU cpu(&memory);

    /*
    Program being loaded into memory:

    MVI A,05
    MVI B,03
    ADD B
    MOV C,A
    SUB B
    HLT

    Expected flow:
    A = 05
    B = 03
    A = A + B  -> 08
    C = A      -> 08
    A = A - B  -> 05
    */

    memory.write(0x0000, 0x3E); // MVI A
    memory.write(0x0001, 0x05);

    memory.write(0x0002, 0x06); // MVI B
    memory.write(0x0003, 0x03);

    memory.write(0x0004, 0x80); // ADD B

    memory.write(0x0005, 0x4F); // MOV C,A

    memory.write(0x0006, 0x90); // SUB B

    memory.write(0x0007, 0x76); // HLT

    cpu.run();

    return 0;
}
