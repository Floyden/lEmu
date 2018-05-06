#include "CPU.h"
#include <fstream>
#include <iterator>
#include <vector>
#include <cstring>
#include <iostream>

// workaround name because and is a c++ keyword
void and_(CPU* cpu, u8 val)
{
    cpu->regs.a &= val;

    cpu->regs.clearAllFlags();
    if(!cpu->regs.a)
        cpu->regs.setZeroFlag();
    cpu->regs.setHalfCarryFlag();
}

void xor_(CPU* cpu, u8 val)
{
    cpu->regs.a ^= val;

    cpu->regs.clearAllFlags();
    if(!cpu->regs.a)
        cpu->regs.setZeroFlag();
}

// 0x00
void nop(CPU*, u16) {};

// 0x01
void ld_bc_nn(CPU* cpu, u16 val) { cpu->regs.bc = val; }

// 0x02
void ld_bc_a(CPU* cpu, u16) { cpu->mem->writeByte(cpu->regs.bc, cpu->regs.a); }

// 0x0e
void ld_c_n(CPU* cpu, u16 val) { cpu->regs.c = (u8) val; }

// 0x0F
void rrca(CPU* cpu, u16 val)
{
    cpu->regs.clearAllFlags();
    cpu->regs.f = (cpu->regs.a & 0x1) << 4;
    cpu->regs.a = cpu->regs.a >> 1;
}

// 0x11
void ld_de_nn(CPU* cpu, u16 val) { cpu->regs.de = val; }

// 0x12
void ld_DE_a(CPU* cpu, u16) { cpu->mem->writeByte(cpu->regs.de, cpu->regs.a); }

// 0x1E
void ld_e_n(CPU* cpu, u16 val) { cpu->regs.e = (u8) val; }

// 0x21
void ld_hl_nn(CPU* cpu, u16 val) { cpu->regs.hl = val; }

// 0x2A
void ldi_a_hl(CPU* cpu, u16) { cpu->regs.a = cpu->mem->readByte(cpu->regs.hl++); }

// 0x2E
void ld_l_n(CPU* cpu, u16 val) { cpu->regs.l = (u8) val; }

// 0x31
void ld_sp_nn(CPU* cpu, u16 val) { cpu->regs.sp = val; }

// 0x3e
void ld_a_n(CPU* cpu, u16 val) { cpu->regs.a = (u8) val; }

// 0x41
void ld_b_c(CPU* cpu, u16) { cpu->regs.b = cpu->regs.c; }

// 0xAF
void xor_a(CPU* cpu, u16) { xor_(cpu, cpu->regs.a); }

// 0xC3
void jp_a16(CPU* cpu, u16 addr) { cpu->regs.pc = addr; };

// 0xC9
void ret(CPU* cpu, u16)
{
    u16 addr = cpu->mem->readShort(cpu->regs.sp);
    cpu->regs.sp += 2;
    cpu->regs.pc = addr;
};

// 0xE0
void ldh_n_a(CPU* cpu, u16 val) { cpu->mem->writeByte( 0xFF00 | ((u8) val), cpu->regs.a); }

// 0xE6
void and_a_n(CPU* cpu, u16 val) { and_(cpu, (u8)val); };

// 0xF0
void ldh_a_n(CPU* cpu, u16 val) { cpu->regs.a = cpu->mem->readByte( 0xFF00 | ((u8) val)); }

// 0xFA
void ld_a_nn(CPU* cpu, u16 val) { cpu->regs.a = cpu->mem->readByte(val); }

// 0xFE
void xor_n(CPU* cpu, u16 val) { xor_(cpu, (u8) val); }

struct OpCode
{
    OpCode(String code, u8 length, u8 ticks, void (*fn)(CPU *, u16)) :
        code(code),
        length(length),
        ticks(ticks),
        fn(fn)
        {};
    String code;
    u8 length;
    u8 ticks;
    void (*fn)(CPU *, u16);
};


static const OpCode OP_CODES[] =
{
    OpCode("NOP", 1, 4, nop),               // 0x00
    OpCode("LD BC, d16", 3, 12, ld_bc_nn),  // 0x01
    OpCode("LD (BC), A", 1, 8, ld_bc_a),    // 0x02
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("LD C,d8", 2, 8, ld_c_n),        // 0x0E
    OpCode("RRCA", 1, 4, rrca),             // 0x0F
    OpCode("", 0, 0, nullptr),  // 0x10
    OpCode("LD DE, d16", 3, 12, ld_de_nn),  // 0x11
    OpCode("LD (DE), A", 1, 8, ld_de_a),    // 0x12
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("LD E,d8", 2, 8, ld_e_n),        // 0x1E
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0x20
    OpCode("LD hl, d16", 3, 12, ld_hl_nn),  // 0x21
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("LDI A, (HL)", 1, 8, ldi_a_hl),  // 0x2A
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("LD L,d8", 2, 8, ld_l_n),        // 0x2E
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0x30
    OpCode("LD SP, d16", 3, 12, ld_sp_nn),  // 0x31
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("LD A,d8", 2, 8, ld_a_n),        // 0x3E
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),              // 0x40
    OpCode("LD B, C", 1, 4, ld_b_c),        // 0x41
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0x50
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0x60
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0x70
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0x80
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0x90
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0xA0
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("XOR A", 1, 4, xor_a),       // 0xAF
    OpCode("", 0, 0, nullptr),  // 0xB0
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0xC0
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("JP (a16)", 3, 16, jp_a16),      // 0xC3
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("RET", 1, 16, ret),              // 0xC9
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),  // 0xD0
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("LDH (a8), A", 2, 12, ldh_n_a),  // 0xF0
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("AND A, n", 2, 8, and_a_n),      // 0xE6
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("LDH A, (a8)", 2, 12, ldh_a_n),  // 0xF0
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("LD A,(a16)", 3, 16, ld_a_nn),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("", 0, 0, nullptr),
    OpCode("XOR d8", 2, 8, xor_n),       // 0xFE
    OpCode("", 0, 0, nullptr),
};


Vec<u8> LoadRom(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    std::vector<u8> buffer(
        (std::istream_iterator<u8>(file)),
        (std::istream_iterator<u8>())
    );

    return buffer;
}

CPU::~CPU()
{
    if(mem)
        delete mem;
}

void CPU::init()
{
    mem = new MemoryMap();

    //just load tetris for now
    loaded_rom = LoadRom("Tetris.gb");
    memcpy(&mem->memory, loaded_rom.data(), 32 * 1024);

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
    mem->memory[0xFF05] = 0x00;
    mem->memory[0xFF06] = 0x00;
    mem->memory[0xFF07] = 0x00;
    mem->memory[0xFF07] = 0x00;
    mem->memory[0xFF10] = 0x80;
    mem->memory[0xFF11] = 0xBF;
    mem->memory[0xFF12] = 0xF3;
    mem->memory[0xFF14] = 0xBF;
    mem->memory[0xFF16] = 0x3F;
    mem->memory[0xFF17] = 0x00;
    mem->memory[0xFF19] = 0xBF;
    mem->memory[0xFF1A] = 0x7F;
    mem->memory[0xFF1B] = 0xFF;
    mem->memory[0xFF1C] = 0x9F;
    mem->memory[0xFF1E] = 0xBF;
    mem->memory[0xFF20] = 0xFF;
    mem->memory[0xFF21] = 0x00;
    mem->memory[0xFF22] = 0x00;
    mem->memory[0xFF23] = 0xBF;
    mem->memory[0xFF24] = 0x77;
    mem->memory[0xFF25] = 0xF3;
    mem->memory[0xFF26] = 0xF1;
    mem->memory[0xFF40] = 0x91;
    mem->memory[0xFF42] = 0x00;
    mem->memory[0xFF43] = 0x00;
    mem->memory[0xFF45] = 0x00;
    mem->memory[0xFF47] = 0xFC;
    mem->memory[0xFF48] = 0xFF;
    mem->memory[0xFF49] = 0xFF;
    mem->memory[0xFF4A] = 0x00;
    mem->memory[0xFF4B] = 0x00;
    mem->memory[0xFFFF] = 0x00;
}

void CPU::run()
{
    while(true)
        step();
}

void CPU::step()
{
    u8 cmd = mem->memory[regs.pc];
    if(OP_CODES[cmd].fn == nullptr)
    {
        printf("Undefined OpCode: %s [%hhx], at location %hx\n",
            OP_CODES[cmd], cmd, regs.pc);
        exit(1);
    }

    u16 arg = 0;
    if(OP_CODES[cmd].length == 2)
        arg = mem->readByte(regs.pc + 1);
    else if(OP_CODES[cmd].length == 3)
        arg = mem->readShort(regs.pc + 1);
    // printf("%hx\n", arg);


    this->regs.pc += OP_CODES[cmd].length;
    (OP_CODES[cmd].fn)(this, arg);

}
