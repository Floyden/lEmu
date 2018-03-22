#pragma once
#include <cstdint>



struct MemoryMap
{
    uint8_t loadRomBank0(const uint8_t* data, uint32_t size);
    uint8_t loadRomBank(const uint8_t* data, uint32_t size);

    union
    {
        struct
        {
            uint8_t bank[0x4000];
            uint8_t sw_bank[0x4000];
            uint8_t vram[0x2000];
            uint8_t ex_ram[0x2000];
            uint8_t wram0[0x1000];
            uint8_t wram1[0x1000];
            uint8_t echo[0x1E00];
            uint8_t oam[0x00A0];
            uint8_t _reserved[0x0060];
            uint8_t io_ports[0x0080];
            uint8_t hram[0x007F];
            uint8_t interrupt_enable;
        };
        uint8_t memory[0x10000];
    };
};
