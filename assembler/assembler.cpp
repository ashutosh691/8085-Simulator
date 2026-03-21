#include "memory.h"
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>
#include <vector>

static std::string trim(const std::string& s)
{
    size_t start = s.find_first_not_of(" \t");
    size_t end   = s.find_last_not_of(" \t");
    if(start == std::string::npos) return "";
    return s.substr(start,end-start+1);
}

void assembleProgram(Memory& memory, const std::string& filename)
{
    std::ifstream file(filename);

    if(!file.is_open())
    {
        std::cerr << "Error: could not open assembly file\n";
        return;
    }

    std::vector<std::string> lines;
    std::map<std::string,int> labels;

    std::string line;
    int address = 0;

    /* PASS 1 — collect labels */
    while(std::getline(file,line))
    {
        line = trim(line);
        if(line.empty()) continue;

        if(line.back() == ':')
        {
            std::string label = line.substr(0,line.size()-1);
            labels[label] = address;
            continue;
        }

        lines.push_back(line);

        if(line.find("MVI") == 0)
            address += 2;
        else if(line.find("ADI") == 0 ||
                 line.find("SUI") == 0 ||
                 line.find("ANI") == 0 ||
                 line.find("ORI") == 0)
        {
            address += 2;
        }
        else if(line.find("JMP") == 0 ||
                 line.find("JZ")  == 0 ||
                 line.find("JNZ") == 0)
            address += 3;
        else if(line.find("LDA") == 0 ||
                 line.find("STA") == 0 ||
                 line.find("LHLD") == 0 ||
                 line.find("SHLD") == 0)
        {
            address += 3;
        }
        else
            address += 1;
    }

    file.close();

    /* PASS 2 — generate machine code */
    address = 0;

    for(const auto& line : lines)
    {
        if(line == "HLT")
        {
            memory.write(address++,0x76);
        }
        else if(line == "ADD B")
        {
            memory.write(address++,0x80);
        }
        else if(line == "DCR B")
        {
            memory.write(address++,0x05);
        }
        else if(line.find("MVI") == 0)
        {
            std::stringstream ss(line);
            std::string ins,reg,value;

            ss >> ins;
            std::getline(ss,reg,',');
            reg = trim(reg);
            ss >> value;

            if(reg=="A") memory.write(address++,0x3E);
            else if(reg=="B") memory.write(address++,0x06);

            memory.write(address++,std::stoi(value,nullptr,16));
        }
        else if(line.find("JNZ") == 0)
        {
            std::stringstream ss(line);
            std::string ins,label;

            ss >> ins >> label;

            int target = labels[label];

            memory.write(address++,0xC2);
            memory.write(address++, target & 0xFF);
            memory.write(address++, (target>>8) & 0xFF);
        }
        else if(line.find("ADI") == 0)
        {
            std::stringstream ss(line);
            std::string ins, value;

            ss >> ins >> value;

            memory.write(address++, 0xC6);
            memory.write(address++, std::stoi(value, nullptr, 16));
        }
        else if(line.find("SUI") == 0)
        {
            std::stringstream ss(line);
            std::string ins, value;

            ss >> ins >> value;

            memory.write(address++, 0xD6);
            memory.write(address++, std::stoi(value, nullptr, 16));
        }

        else if(line.find("ANI") == 0)
        {
            std::stringstream ss(line);
            std::string ins, value;

            ss >> ins >> value;

            memory.write(address++, 0xE6);
            memory.write(address++, std::stoi(value, nullptr, 16));
        }

        else if(line.find("ORI") == 0)
        {
            std::stringstream ss(line);
            std::string ins, value;

            ss >> ins >> value;

            memory.write(address++, 0xF6);
            memory.write(address++, std::stoi(value, nullptr, 16));
        }
        else if(line.find("LDA") == 0)
        {
            std::stringstream ss(line);
            std::string ins, addr_str;

            ss >> ins >> addr_str;

            int addr = std::stoi(addr_str, nullptr, 16);

            memory.write(address++, 0x3A);
            memory.write(address++, addr & 0xFF);         // low byte
            memory.write(address++, (addr >> 8) & 0xFF);  // high byte
        }
        else if(line.find("STA") == 0)
        {
            std::stringstream ss(line);
            std::string ins, addr_str;

            ss >> ins >> addr_str;

            int addr = std::stoi(addr_str, nullptr, 16);

            memory.write(address++, 0x32);
            memory.write(address++, addr & 0xFF);
            memory.write(address++, (addr >> 8) & 0xFF);
        }
        else if(line.find("LHLD") == 0)
        {
            std::stringstream ss(line);
            std::string ins, addr_str;

            ss >> ins >> addr_str;

            int addr = std::stoi(addr_str, nullptr, 16);

            memory.write(address++, 0x2A);
            memory.write(address++, addr & 0xFF);
            memory.write(address++, (addr >> 8) & 0xFF);
        }
        else if(line.find("SHLD") == 0)
        {
            std::stringstream ss(line);
            std::string ins, addr_str;

            ss >> ins >> addr_str;

            int addr = std::stoi(addr_str, nullptr, 16);

            memory.write(address++, 0x22);
            memory.write(address++, addr & 0xFF);
            memory.write(address++, (addr >> 8) & 0xFF);
        }
    }

    std::cout<<"Assembly completed successfully\n";
}
