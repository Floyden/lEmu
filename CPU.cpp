#include "CPU.h"
#include <fstream>
#include <iterator>
#include <vector>
#include <cstring>
#include <iostream>

//length, cycles
std::pair<uint8_t, uint8_t> opcode_table[] =
{
    {1,4} // NOOP
};

std::vector<char> LoadRom(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    std::vector<char> buffer(
        (std::istream_iterator<char>(file)),
        (std::istream_iterator<char>())
    );

    return buffer;
}

CPU::~CPU()
{
    if(map)
        delete map;
}

void CPU::init()
{
    map = new MemoryMap();

    //just load tetris for now
    loaded_rom = LoadRom("Tetris.gb");
    memcpy(&map->memory, loaded_rom.data(), 32 * 1024);

    //set registers
    regs.a = 0x01;
    regs.f = 0xB0;
    regs.bc = 0x13;
    regs.de = 0xD8;
    regs.hl = 0x14D;
    regs.sp = 0xFFFE;
    //skip validation
    regs.pc = 0x100;

    // set stack
    map->memory[0xFF05] = 0x00;
    map->memory[0xFF06] = 0x00;
    map->memory[0xFF07] = 0x00;
    map->memory[0xFF07] = 0x00;
    map->memory[0xFF10] = 0x80;
    map->memory[0xFF11] = 0xBF;
    map->memory[0xFF12] = 0xF3;
    map->memory[0xFF14] = 0xBF;
    map->memory[0xFF16] = 0x3F;
    map->memory[0xFF17] = 0x00;
    map->memory[0xFF19] = 0xBF;
    map->memory[0xFF1A] = 0x7F;
    map->memory[0xFF1B] = 0xFF;
    map->memory[0xFF1C] = 0x9F;
    map->memory[0xFF1E] = 0xBF;
    map->memory[0xFF20] = 0xFF;
    map->memory[0xFF21] = 0x00;
    map->memory[0xFF22] = 0x00;
    map->memory[0xFF23] = 0xBF;
    map->memory[0xFF24] = 0x77;
    map->memory[0xFF25] = 0xF3;
    map->memory[0xFF26] = 0xF1;
    map->memory[0xFF40] = 0x91;
    map->memory[0xFF42] = 0x00;
    map->memory[0xFF43] = 0x00;
    map->memory[0xFF45] = 0x00;
    map->memory[0xFF47] = 0xFC;
    map->memory[0xFF48] = 0xFF;
    map->memory[0xFF49] = 0xFF;
    map->memory[0xFF4A] = 0x00;
    map->memory[0xFF4B] = 0x00;
    map->memory[0xFFFF] = 0x00;
}

void CPU::run()
{
    decode(regs.pc);
    decode(regs.pc);
}

void CPU::decode(uint16_t location)
{
    uint8_t cmd = map->memory[location];
    switch (cmd)
    {
        case 0x00:
            break;
        default:
            printf("Unknown code: %hhx\n", cmd);
            exit(1);
    }
    regs.pc += opcode_table[cmd].first;
}
