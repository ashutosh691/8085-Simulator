#include "cpu.h"
#include "memory.h"

void assembleProgram(Memory& memory, const std::string& filename);

int main()
{
    Memory memory;
    CPU cpu(&memory);

    assembleProgram(memory, "program.asm");

    cpu.run();

    return 0;
}
