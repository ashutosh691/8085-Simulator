#include "memory.h"
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>

void assembleProgram(Memory& memory, const std::string& filename)
{
    std::ifstream file(filename);

    if(!file.is_open())
    {
        std::cerr << "Error: could not open assembly file\n";
        return;
    }

    std::string line;
    int address = 0;

    const std::map<std::string, uint8_t> opcode = {
        {"HLT", 0x76},
        {"ADD B", 0x80},
        {"ADD C", 0x81},
        {"ADD D", 0x82},
        {"ADD E", 0x83},
        {"ADD H", 0x84},
        {"ADD L", 0x85},
        {"ADD A", 0x87}
    };

    while(std::getline(file,line))
    {
        if(line.empty())
            continue;

        /* Check simple instructions */
        if(opcode.count(line))
        {
            memory.write(address++, opcode.at(line));
            continue;
        }

        /* Parse MVI instruction */
        if(line.find("MVI") == 0)
        {
            std::string reg;
            std::string value;

            std::stringstream ss(line);
            std::string instruction;

            ss >> instruction; // MVI

            std::getline(ss, reg, ',');   // read register
            reg.erase(0,1);               // remove leading space

            ss >> value;                  // read value

            if(reg == "A")
                memory.write(address++, 0x3E);
            else if(reg == "B")
                memory.write(address++, 0x06);
            else if(reg == "C")
                memory.write(address++, 0x0E);
            else if(reg == "D")
                memory.write(address++, 0x16);
            else if(reg == "E")
                memory.write(address++, 0x1E);
            else if(reg == "H")
                memory.write(address++, 0x26);
            else if(reg == "L")
                memory.write(address++, 0x2E);
            else
            {
                std::cerr << "Unsupported register in MVI\n";
                continue;
            }

            memory.write(address++, std::stoi(value,nullptr,16));
        }
    }

    std::cout << "Assembly completed successfully\n";
}
