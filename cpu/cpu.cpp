#include "cpu.h"
#include "memory.h"
#include <iostream>
#include <fstream>
#include <sstream>

CPU::CPU(Memory* mem) : memory(mem)
{
    A = B = C = D = E = H = L = 0;
    PC = 0;
    SP = 0;
    halted = false;

    flags = {false,false,false,false,false};
}

void CPU::updateArithmeticFlags(uint16_t result)
{
    uint8_t value = result & 0xFF;

    flags.CY = (result > 0xFF);
    flags.Z  = (value == 0);
    flags.S  = (value & 0x80) != 0;
    flags.P  = calculateParity(value);
}

bool CPU::calculateParity(uint8_t value)
{
    int count = 0;

    for(int i=0;i<8;i++)
        if(value & (1<<i))
            count++;

    return (count % 2 == 0);
}

uint8_t CPU::getRegister(uint8_t code)
{
    switch(code)
    {
    case 0: return B;
    case 1: return C;
    case 2: return D;
    case 3: return E;
    case 4: return H;
    case 5: return L;
    case 7: return A;
    default: return 0;
    }
}

void CPU::setRegister(uint8_t code, uint8_t value)
{
    switch(code)
    {
    case 0: B = value; break;
    case 1: C = value; break;
    case 2: D = value; break;
    case 3: E = value; break;
    case 4: H = value; break;
    case 5: L = value; break;
    case 7: A = value; break;
    }
}

uint8_t CPU::readFromCode(uint8_t code)
{
    if(code == 6)
    {
        uint16_t address = (H<<8) | L;
        return memory->read(address);
    }

    return getRegister(code);
}

void CPU::writeFromCode(uint8_t code, uint8_t value)
{
    if(code == 6)
    {
        uint16_t address = (H<<8) | L;
        memory->write(address,value);
    }
    else
        setRegister(code,value);
}

void CPU::dumpRegisters()
{
    std::cout << "A=" << std::hex << (int)A << "\n";
    std::cout << "B=" << std::hex << (int)B << "\n";
    std::cout << "C=" << std::hex << (int)C << "\n";
    std::cout << "D=" << std::hex << (int)D << "\n";
    std::cout << "E=" << std::hex << (int)E << "\n";
    std::cout << "H=" << std::hex << (int)H << "\n";
    std::cout << "L=" << std::hex << (int)L << "\n";
}

/*
---------------------------------------------------------
RUN LOOP
---------------------------------------------------------
*/

void CPU::run()
{
    while(!halted)
    {
        step();
    }

    dumpRegisters();
}

/*
---------------------------------------------------------
STEP
Executes exactly ONE instruction
---------------------------------------------------------
*/

void CPU::step()
{
    uint8_t opcode = memory->read(PC++);

    /* NOP */
    if(opcode == 0x00)
    {
        return;
    }

    /* HLT */
    if(opcode == 0x76)
    {
        std::cout<<"HLT encountered\n";
        halted = true;
        return;
    }

    /* MVI r,data */
    if((opcode & 0xC7) == 0x06)
    {
        uint8_t reg = (opcode>>3) & 7;
        uint8_t value = memory->read(PC++);
        writeFromCode(reg,value);
        return;
    }

    /* MOV r1,r2 */
    if((opcode & 0xC0) == 0x40)
    {
        uint8_t dest = (opcode>>3) & 7;
        uint8_t src  = opcode & 7;

        uint8_t value = readFromCode(src);
        writeFromCode(dest,value);
        return;
    }

    /* INR */
    if ((opcode & 0xC7) == 0x04)
    {
        uint8_t reg = (opcode >> 3) & 7;

        uint8_t value = readFromCode(reg);
        value++;

        writeFromCode(reg, value);
        updateArithmeticFlags(value);

        return;
    }

    /* DCR */
    if ((opcode & 0xC7) == 0x05)
    {
        uint8_t reg = (opcode >> 3) & 7;

        uint8_t value = readFromCode(reg);
        value--;

        writeFromCode(reg, value);
        updateArithmeticFlags(value);

        return;
    }

    /* ADD */
    if((opcode & 0xF8) == 0x80)
    {
        uint8_t src = opcode & 7;
        uint8_t value = readFromCode(src);

        uint16_t result = A + value;

        updateArithmeticFlags(result);
        A = result & 0xFF;

        return;
    }

    /* SUB */
    if((opcode & 0xF8) == 0x90)
    {
        uint8_t src = opcode & 7;
        uint8_t value = readFromCode(src);

        uint16_t result = A - value;

        updateArithmeticFlags(result);
        A = result & 0xFF;

        return;
    }

    /* ANA */
    if ((opcode & 0xF8) == 0xA0)
    {
        uint8_t src = opcode & 7;
        uint8_t value = readFromCode(src);

        A = A & value;

        flags.CY = 0;
        flags.Z = (A == 0);
        flags.S = (A & 0x80) != 0;
        flags.P = calculateParity(A);

        return;
    }

    /* XRA */
    if ((opcode & 0xF8) == 0xA8)
    {
        uint8_t src = opcode & 7;
        uint8_t value = readFromCode(src);

        A = A ^ value;

        flags.CY = 0;
        flags.Z = (A == 0);
        flags.S = (A & 0x80) != 0;
        flags.P = calculateParity(A);

        return;
    }

    /* ORA */
    if ((opcode & 0xF8) == 0xB0)
    {
        uint8_t src = opcode & 7;
        uint8_t value = readFromCode(src);

        A = A | value;

        flags.CY = 0;
        flags.Z = (A == 0);
        flags.S = (A & 0x80) != 0;
        flags.P = calculateParity(A);

        return;
    }

    /* CMP */
    if ((opcode & 0xF8) == 0xB8)
    {
        uint8_t src = opcode & 7;
        uint8_t value = readFromCode(src);

        uint16_t result = A - value;

        updateArithmeticFlags(result);

        return;
    }

    /* JMP */
    if (opcode == 0xC3)
    {
        uint8_t low = memory->read(PC++);
        uint8_t high = memory->read(PC++);

        PC = (high << 8) | low;
        return;
    }

    /* JZ */
    if (opcode == 0xCA)
    {
        uint8_t low = memory->read(PC++);
        uint8_t high = memory->read(PC++);

        uint16_t address = (high << 8) | low;

        if(flags.Z)
            PC = address;

        return;
    }

    /* JNZ */
    if (opcode == 0xC2)
    {
        uint8_t low = memory->read(PC++);
        uint8_t high = memory->read(PC++);

        uint16_t address = (high << 8) | low;

        if(!flags.Z)
            PC = address;

        return;
    }

    /* CALL */
    if (opcode == 0xCD)
    {
        uint8_t low = memory->read(PC++);
        uint8_t high = memory->read(PC++);

        uint16_t address = (high << 8) | low;

        memory->write(--SP, (PC >> 8) & 0xFF);
        memory->write(--SP, PC & 0xFF);

        PC = address;
        return;
    }

    /* RET */
    if (opcode == 0xC9)
    {
        uint8_t low = memory->read(SP++);
        uint8_t high = memory->read(SP++);

        PC = (high << 8) | low;
        return;
    }

    /* ADI */
    if (opcode == 0xC6)
    {
        uint8_t value = memory->read(PC++);

        uint16_t result = A + value;

        updateArithmeticFlags(result);
        A = result & 0xFF;

        return;
    }

    /* SUI */
    if (opcode == 0xD6)
    {
        uint8_t value = memory->read(PC++);

        uint16_t result = A - value;

        updateArithmeticFlags(result);
        A = result & 0xFF;

        return;
    }

    /* ANI */
    if (opcode == 0xE6)
    {
        uint8_t value = memory->read(PC++);

        A = A & value;

        flags.Z = (A == 0);
        flags.S = (A & 0x80) != 0;
        flags.P = calculateParity(A);
        flags.CY = 0;

        return;
    }

    /* ORI */
    if (opcode == 0xF6)
    {
        uint8_t value = memory->read(PC++);

        A = A | value;

        flags.Z = (A == 0);
        flags.S = (A & 0x80) != 0;
        flags.P = calculateParity(A);
        flags.CY = 0;

        return;
    }

    std::cout<<"Unknown opcode: "<<std::hex<<(int)opcode<<"\n";
    halted = true;
}
