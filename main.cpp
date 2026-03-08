#include "cpu.h"
#include "memory.h"
#include <iostream>

int main()
{
    Memory memory;
    CPU cpu(&memory);

    /*
    Program layout

    0000  MVI A,05
    0002  MVI B,03
    0004  ADD B
    0005  MOV C,A
    0006  INR C
    0007  DCR B
    0008  ANA C
    0009  XRA B
    000A  ORA C
    000B  CMP B
    000C  JZ 0015
    000F  ADI 02
    0011  CALL 0020
    0014  JMP 0025

    0015  MVI A,FF
    0017  JMP 0025

    0020  INR A
    0021  INR A
    0022  RET

    0025  HLT
    */

    memory.write(0x0000,0x3E); // MVI A,05
    memory.write(0x0001,0x05);

    memory.write(0x0002,0x06); // MVI B,03
    memory.write(0x0003,0x03);

    memory.write(0x0004,0x80); // ADD B

    memory.write(0x0005,0x4F); // MOV C,A

    memory.write(0x0006,0x0C); // INR C

    memory.write(0x0007,0x05); // DCR B

    memory.write(0x0008,0xA1); // ANA C

    memory.write(0x0009,0xA8); // XRA B

    memory.write(0x000A,0xB1); // ORA C

    memory.write(0x000B,0xB8); // CMP B

    memory.write(0x000C,0xCA); // JZ 0015
    memory.write(0x000D,0x15);
    memory.write(0x000E,0x00);

    memory.write(0x000F,0xC6); // ADI 02
    memory.write(0x0010,0x02);

    memory.write(0x0011,0xCD); // CALL 0020
    memory.write(0x0012,0x20);
    memory.write(0x0013,0x00);

    memory.write(0x0014,0xC3); // JMP 0025
    memory.write(0x0015,0x25);
    memory.write(0x0016,0x00);

    memory.write(0x0015,0x3E); // MVI A,FF
    memory.write(0x0016,0xFF);

    memory.write(0x0017,0xC3); // JMP 0025
    memory.write(0x0018,0x25);
    memory.write(0x0019,0x00);

    memory.write(0x0020,0x3C); // INR A
    memory.write(0x0021,0x3C); // INR A
    memory.write(0x0022,0xC9); // RET

    memory.write(0x0025,0x76); // HLT

    cpu.run();

    return 0;
}
