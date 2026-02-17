#include "cpu.h"
#include "memory.h"
#include <iostream>

int main()
{
    Memory memory;
    CPU cpu(&memory);

    memory.write(0x0000, 0x3E);  // random test opcode

    cpu.execute();

    return 0;
}

