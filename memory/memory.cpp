#include "memory.h"

Memory::Memory() : storage(MEMORY_SIZE, 0) {}

uint8_t Memory::read(uint16_t address) const
{
    return storage[address];
}

void Memory::write(uint16_t address, uint8_t value)
{
    storage[address] = value;
}
