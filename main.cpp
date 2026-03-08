#include "cpu.h"
#include "memory.h"
#include <iostream>
#include <fstream>
#include <sstream>

void loadProgram(Memory& memory, const std::string& filename)
{
    std::ifstream file(filename);

    if(!file.is_open())
    {
        std::cerr << "Error: Could not open program file\n";
        return;
    }

    std::string byte;
    int address = 0;

    while(file >> byte)
    {
        uint8_t value = std::stoi(byte, nullptr, 16);

        memory.write(address, value);

        std::cout << "Loaded " << byte
                  << " at address "
                  << std::hex << address << "\n";

        address++;
    }

    std::cout << "Program loaded successfully\n";
}

int main()
{
    Memory memory;
    CPU cpu(&memory);

    loadProgram(memory, "../program.hex");

    std::cout << "\nStarting CPU execution...\n\n";

    cpu.run();

    return 0;
}
