#include "cpu.h"
#include "memory.h"
#include <iostream>

CPU::CPU(Memory* mem) : memory(mem)
{
    A = B = C = D = E = H = L = 0;
    PC = 0;
    SP = 0;
    halted = false;

    flags = {false, false, false, false, false};
}

void CPU::updateArithmeticFlags(uint16_t result)
{
    uint8_t value = result & 0xFF;

    flags.CY = (result > 0xFF);
    flags.Z  = (value == 0);
    flags.S  = (value & 0x80);

    flags.P = calculateParity(value);
}

bool CPU::calculateParity(uint8_t value)
{
    int count = 0;
    for(int i = 0; i < 8; i++)
        if(value & (1 << i))
            count++;

    return (count % 2 == 0);
}

void CPU::run()
{
    while (!halted)
    {
        uint8_t opcode = memory->read(PC++);

        switch (opcode)
        {
        case 0x76:   // HLT opcode in 8085
            std::cout << "HLT encountered. Stopping execution.\n";
            halted = true;
            break;

        case 0x3E:   // MVI A, immediate
        {
            uint8_t value = memory->read(PC++);
            A = value;

            std::cout << "MVI A executed. A = 0x"
                      << std::hex << (int)A << std::endl;
            break;
        }

        case 0x06:   // MVI B, immediate
        {
            uint8_t value = memory->read(PC++);
            B = value;

            std::cout << "MVI B executed. B = 0x"
                      << std::hex << (int)B << std::endl;
            break;
        }

        case 0x0E:   // MVI C, immediate
        {
            uint8_t value = memory->read(PC++);
            C = value;

            std::cout << "MVI C executed. C = 0x"
                      << std::hex << (int)C << std::endl;
            break;
        }

        case 0x16:   // MVI D, immediate
        {
            uint8_t value = memory->read(PC++);
            D = value;

            std::cout << "MVI D executed. D = 0x"
                      << std::hex << (int)D << std::endl;
            break;
        }

        case 0x1E:   // MVI E, immediate
        {
            uint8_t value = memory->read(PC++);
            E = value;

            std::cout << "MVI E executed. E = 0x"
                      << std::hex << (int)E << std::endl;
            break;
        }

        case 0x26:   // MVI H, immediate
        {
            uint8_t value = memory->read(PC++);
            H = value;

            std::cout << "MVI H executed. H = 0x"
                      << std::hex << (int)H << std::endl;
            break;
        }

        case 0x2E:   // MVI L, immediate
        {
            uint8_t value = memory->read(PC++);
            L = value;

            std::cout << "MVI L executed. L = 0x"
                      << std::hex << (int)L << std::endl;
            break;
        }

        case 0x80:   // ADD B
        {
            uint16_t result = A + B;
            updateArithmeticFlags(result);
            A = result & 0xFF;

            std::cout << "ADD B executed. A = 0x"
                      << std::hex << (int)A << std::endl;
            break;
        }



        default:
            std::cout << "Unknown opcode: 0x"
                      << std::hex << (int)opcode << std::endl;
            halted = true;
            break;
        }
    }
    std::cout << "Flags -> "
              << "Z: " << flags.Z << " "
              << "S: " << flags.S << " "
              << "P: " << flags.P << " "
              << "CY: " << flags.CY << "\n";
}
