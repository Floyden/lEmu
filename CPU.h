#pragma once
#include <cstdint>
#include <vector>
#include "Memory.h"

struct Registers
{
    union
    {
        struct
        {
            uint8_t f;
            uint8_t a;
        };
        uint16_t af;
    };

    union
    {
        struct
        {
            uint8_t c;
            uint8_t b;
        };
        uint16_t bc;
    };
    union
    {
        struct
        {
            uint8_t e;
            uint8_t d;
        };
        uint16_t de;
    };
    union
    {
        struct
        {
            uint8_t l;
            uint8_t h;
        };
        uint16_t hl;
    };

    uint16_t sp;
    uint16_t pc;
};

class CPU
{
public:
    ~CPU();
    void init();
    void run();

    void decode(uint16_t);

    Registers regs;
    MemoryMap* map;
    std::vector<char> loaded_rom;
};
