#pragma once
#include "Prerequisites.h"
#include "Memory.h"

struct Registers
{
    union
    {
        struct
        {
            u8 f;
            u8 a;
        };
        u16 af;
    };

    union
    {
        struct
        {
            u8 c;
            u8 b;
        };
        u16 bc;
    };
    union
    {
        struct
        {
            u8 e;
            u8 d;
        };
        u16 de;
    };
    union
    {
        struct
        {
            u8 l;
            u8 h;
        };
        u16 hl;
    };

    u16 sp;
    u16 pc;

    void setZeroFlag()          { f |= 0x80; }
    void setSubstractFlag()     { f |= 0x40; }
    void setHalfCarryFlag()     { f |= 0x20; }
    void setCarryFlag()         { f |= 0x10; }

    void clearZeroFlag()        { f &= (~0x80); }
    void clearSubstractFlag()   { f &= (~0x40); }
    void clearHalfCarryFlag()   { f &= (~0x20); }
    void clearCarryFlag()       { f &= (~0x10); }

    void setAllFlags()          { f = 0xF0; }
    void clearAllFlags()        { f = 0; }
};

class CPU
{
public:
    CPU() {};
    ~CPU();
    void init();
    void run();

    void step();

    Registers regs;
    MemoryMap* mem;
    Vec<u8> loaded_rom;

private:
    CPU(const CPU&) = delete;
};
