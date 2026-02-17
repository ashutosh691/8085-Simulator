#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <cstdint>

constexpr std::size_t MEMORY_SIZE = 65536;

class Memory
{
private:
    std::vector<uint8_t> storage;

public:
    Memory();

    uint8_t read(uint16_t address) const;
    void write(uint16_t address, uint8_t value);
};

#endif // MEMORY_H
