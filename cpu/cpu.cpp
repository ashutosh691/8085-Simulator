#include "cpu.h"
#include "memory.h"
#include <iostream>

/*
---------------------------------------------------------
CPU CONSTRUCTOR

Initializes registers and connects the CPU with memory.
All registers start at 0 and CPU is not halted.
---------------------------------------------------------
*/

CPU::CPU(Memory* mem) : memory(mem)
{
    A = B = C = D = E = H = L = 0;

    PC = 0;
    SP = 0;

    halted = false;

    flags = {false,false,false,false,false};
}

/*
---------------------------------------------------------
updateArithmeticFlags()

Updates CPU flags after arithmetic operations.
The result is passed as 16-bit so we can detect overflow.
---------------------------------------------------------
*/

void CPU::updateArithmeticFlags(uint16_t result)
{
    uint8_t value = result & 0xFF;

    flags.CY = (result > 0xFF);
    flags.Z  = (value == 0);
    flags.S  = (value & 0x80) != 0;
    flags.P  = calculateParity(value);
}

/*
---------------------------------------------------------
calculateParity()

Counts number of set bits in a byte.
Returns true if number of 1s is even.
---------------------------------------------------------
*/

bool CPU::calculateParity(uint8_t value)
{
    int count = 0;

    for(int i=0;i<8;i++)
        if(value & (1<<i))
            count++;

    return (count % 2 == 0);
}

/*
---------------------------------------------------------
getRegister()

Returns value stored in the register specified by code.
Register codes follow 8085 encoding.

000 -> B
001 -> C
010 -> D
011 -> E
100 -> H
101 -> L
111 -> A
---------------------------------------------------------
*/

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

/*
---------------------------------------------------------
setRegister()

Writes a value into the register identified by code.
---------------------------------------------------------
*/

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

/*
---------------------------------------------------------
readFromCode()

Reads either:
• a register value
• or memory location pointed by HL

Register code 110 represents memory (M).
---------------------------------------------------------
*/

uint8_t CPU::readFromCode(uint8_t code)
{
    if(code == 6)
    {
        uint16_t address = (H<<8) | L;
        return memory->read(address);
    }

    return getRegister(code);
}

/*
---------------------------------------------------------
writeFromCode()

Writes either:
• to register
• or to memory pointed by HL
---------------------------------------------------------
*/

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

/*
---------------------------------------------------------
run()

Main CPU loop implementing the instruction cycle:

1. FETCH  -> read opcode from memory
2. DECODE -> identify instruction type
3. EXECUTE -> perform the instruction

Execution continues until HLT instruction is encountered.
---------------------------------------------------------
*/

void CPU::run()
{
    while(!halted)
    {
        uint8_t opcode = memory->read(PC++);

        /* HLT instruction */
        if(opcode == 0x76)
        {
            std::cout<<"HLT encountered\n";
            halted = true;
            break;
        }

        /* MVI r,data instruction */
        if((opcode & 0xC7) == 0x06)
        {
            uint8_t reg = (opcode>>3) & 7;
            uint8_t value = memory->read(PC++);

            writeFromCode(reg,value);
            continue;
        }

        /* MOV r1,r2 instruction */
        if((opcode & 0xC0) == 0x40 && opcode != 0x76)
        {
            uint8_t dest = (opcode>>3) & 7;
            uint8_t src  = opcode & 7;

            uint8_t value = readFromCode(src);
            writeFromCode(dest,value);

            continue;
        }

        /* ADD instruction group */
        if((opcode & 0xF8) == 0x80)
        {
            uint8_t src = opcode & 7;
            uint8_t value = readFromCode(src);

            uint16_t result = A + value;

            updateArithmeticFlags(result);
            A = result & 0xFF;

            continue;
        }

        /* SUB instruction group */
        if((opcode & 0xF8) == 0x90)
        {
            uint8_t src = opcode & 7;
            uint8_t value = readFromCode(src);

            uint16_t result = A - value;

            updateArithmeticFlags(result);
            A = result & 0xFF;

            continue;
        }

        std::cout<<"Unknown opcode: "<<std::hex<<(int)opcode<<"\n";
        halted = true;
    }

    std::cout<<"Final A="<<std::hex<<(int)A<<"\n";
}
