# 8085 Microprocessor Simulator

A software simulator for the Intel 8085 microprocessor written in C++. 
It emulates instruction execution, register operations, memory access, and processor flags.

## Features
- Instruction decoding
- Register simulation (A, B, C, D, E, H, L)
- Flag handling (Zero, Carry, Sign, Parity)
- Memory simulation
- Execution cycle emulation

## Tech Stack
C++, STL

## How to Run
git clone https://github.com/ashutosh691/8085-Simulator
cd 8085-Simulator
g++ main.cpp -o simulator
./simulator

## Example Program
MVI A, 05
MVI B, 03
ADD B

## Future Improvements
- Debug mode
- Step-by-step execution
- GUI visualization
