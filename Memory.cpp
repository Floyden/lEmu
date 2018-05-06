#include "Memory.h"
#include <cstring>
#include <iostream>


// uint8_t MemoryMap::loadRom(const uint8_t* data, uint32_t size, uint8_t state)
// {
//     if(state == 0)
//     {
//         memcpy(rom.state0,  data, size);
//         return 0;
//     }
//
//     std::cout << "Unsupported state\n";
//     return 1;
// }

u8 MemoryMap::readByte(u16 addr)
{
    return memory[addr];
}

u16 MemoryMap::readShort(u16 addr)
{
    return readByte(addr) | readByte(addr + 1) << 8;
}

u8 MemoryMap::writeByte(u16 addr, u8 val)
{
    memory[addr] = val;
    return readByte(addr);
}
