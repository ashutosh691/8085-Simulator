# 8085 Microprocessor Simulator

A lightweight **Intel 8085 microprocessor simulator written in C++**.
The project emulates the core components of the 8085 CPU, supports execution of assembly programs, and includes a small assembler that converts assembly code into machine code.

This project was built to understand **CPU architecture, instruction decoding, and emulator design**.

---

# Features

### CPU Emulation

The simulator implements the core behavior of the Intel 8085 processor:

* 8-bit registers
  `A B C D E H L`

* 16-bit registers
  `PC (Program Counter)`
  `SP (Stack Pointer)`

* Flags register

  * Zero (Z)
  * Sign (S)
  * Parity (P)
  * Carry (CY)
  * Auxiliary Carry (AC structure defined)

---

### Instruction Execution

The CPU supports execution of several 8085 instruction groups.

#### Data Transfer

* `MOV r1,r2`
* `MVI r,data`

#### Arithmetic

* `ADD r`
* `SUB r`
* `ADI data`
* `SUI data`

#### Logical

* `ANA r`
* `XRA r`
* `ORA r`
* `ANI data`
* `ORI data`
* `CMP r`

#### Register Operations

* `INR r`
* `DCR r`

#### Control Flow

* `JMP addr`
* `JZ addr`
* `JNZ addr`
* `CALL addr`
* `RET`

#### Machine Control

* `NOP`
* `HLT`

---

# Debugging and Trace Output

Each instruction execution prints:

* Program Counter
* Opcode
* Executed instruction
* Register state

Example output:

```
PC=0000 OPCODE=06
MVI executed
A=00
B=05
C=00
D=00
E=00
H=00
L=00
```

This helps trace program execution step-by-step.

---

# Assembler

The project includes a simple **two-pass assembler** that converts assembly programs into machine code.

Supported assembly instructions include:

```
MVI
MOV
ADD
SUB
INR
DCR
JMP
JZ
JNZ
HLT
```

---

# Label Support

The assembler supports **labels for branching**.

Example:

```
MVI B,05

LOOP:
DCR B
JNZ LOOP

HLT
```

The assembler automatically resolves label addresses.

---

# Project Structure

```
8085Simulator
│
├── assembler/
│   └── assembler.cpp
│
├── cpu/
│   ├── cpu.cpp
│   └── cpu.h
│
├── memory/
│   ├── memory.cpp
│   └── memory.h
│
├── program.asm
│
├── main.cpp
└── CMakeLists.txt
```

---

# CPU Architecture

The simulator models a simplified 8085 execution cycle:

```
Fetch → Decode → Execute
```

1. **Fetch**

   * Read opcode from memory using PC.

2. **Decode**

   * Identify instruction type using opcode masks.

3. **Execute**

   * Perform register, memory, or control operation.

---

# Example Assembly Program

```
MVI B,05

LOOP:
DCR B
JNZ LOOP

HLT
```

---

# Example Execution Trace

```
Assembly completed successfully

PC=0000 OPCODE=06
MVI executed

PC=0002 OPCODE=05
DCR executed

PC=0003 OPCODE=C2
JNZ executed
```

The loop continues until register **B becomes zero**, after which the CPU executes `HLT`.

---

# How to Run

1. Build the project

```
mkdir build
cd build
cmake ..
make
```

2. Run the simulator

```
./8085Simulator
```

3. Modify `program.asm` to test your own assembly programs.

---

# Learning Goals

This project demonstrates:

* CPU emulator design
* instruction decoding using bit masks
* assembler implementation
* program counter control
* stack operations
* debugging through execution tracing

---

# Future Improvements

Planned features:

* Memory instructions
  `LDA`, `STA`, `LHLD`, `SHLD`

* Instruction dispatch table (faster decoding)

* Memory viewer and debugger tools

* Graphical interface using **Qt**

* Full 8085 instruction set support

---

# Author

**Ashutosh Upreti**
Computer Science Student
Interested in systems programming, CPU architecture, and low-level software development.

---
