#ifndef CPU_H
#define CPU_H

#include <iostream>
#include <unordered_map>
#include <vector>

namespace cpu {

// 15 bit address
constexpr uint16_t DECODER_SIZE = 32768;

// 15 bit address
constexpr uint16_t PROG_ROM_SIZE = 32768;

// 15 bit address
constexpr uint16_t RAM_SIZE = 32768;

// Register Macros
constexpr uint16_t A_REG = 0x00;
constexpr uint16_t B_REG = 0x01;
constexpr uint16_t L_REG = 0x02;
constexpr uint16_t U_REG = 0x03;

// Micro-Instructions
constexpr uint16_t U_BOOT        = 0x0212;
constexpr uint16_t U_UCODE_RESET = 0xFEFF;
constexpr uint16_t U_FETCH       = 0x0312;
constexpr uint16_t U_NOP         = 0x030A;
constexpr uint16_t U_IMM_TO_REG  = 0x0312;
constexpr uint16_t U_A_EN        = 0x0090;
constexpr uint16_t U_B_EN        = 0x0050;
constexpr uint16_t U_L_EN        = 0x00D0;
constexpr uint16_t U_U_EN        = 0x0030;
constexpr uint16_t U_MOV_A       = 0x091A;
constexpr uint16_t U_MOV_B       = 0xA51A;
constexpr uint16_t U_ADD         = 0x991A;
constexpr uint16_t U_ADDC        = 0x911A;
constexpr uint16_t U_SUB         = 0x611A;
constexpr uint16_t U_SUBC        = 0x691A;
constexpr uint16_t U_AND         = 0xB51A;
constexpr uint16_t U_OR          = 0xE51A;
constexpr uint16_t U_XOR         = 0x651A;
constexpr uint16_t U_SHL         = 0xC91A;
constexpr uint16_t U_SHLC        = 0xC11A;
constexpr uint16_t U_NOT_A       = 0x051A;
constexpr uint16_t U_NOT_B       = 0x551A;
constexpr uint16_t U_CMP         = 0x690A;
constexpr uint16_t U_ST_A        = 0xF50C;
constexpr uint16_t U_ST_B        = 0xA50C;
constexpr uint16_t U_STI         = 0x0304;
constexpr uint16_t U_LD          = 0x0318;
constexpr uint16_t U_B           = 0x03FA;
constexpr uint16_t U_OUTI        = 0x0303;

constexpr uint16_t U_IGNORE_REG_MASK = 0xFE1F;

// Macro-Instructions
constexpr uint8_t M_ADD  = 0;
constexpr uint8_t M_ADDI = 1;
constexpr uint8_t M_ADDC = 2;
constexpr uint8_t M_SUB  = 3;
constexpr uint8_t M_SUBI = 4;
constexpr uint8_t M_SUBC = 5;
constexpr uint8_t M_NOT  = 6;
constexpr uint8_t M_AND  = 7;
constexpr uint8_t M_ANDI = 8;
constexpr uint8_t M_OR   = 9;
constexpr uint8_t M_ORI  = 10;
constexpr uint8_t M_XOR  = 11;
constexpr uint8_t M_XORI = 12;
constexpr uint8_t M_SHL  = 13;
constexpr uint8_t M_SHLC = 14;
constexpr uint8_t M_LDI  = 15;
constexpr uint8_t M_LD   = 16;
constexpr uint8_t M_LDA  = 17;
constexpr uint8_t M_ST   = 18;
constexpr uint8_t M_STA  = 19;
constexpr uint8_t M_STI  = 20;
constexpr uint8_t M_STIA = 21;
constexpr uint8_t M_CMP  = 22;
constexpr uint8_t M_B    = 23;
constexpr uint8_t M_BC   = 24;
constexpr uint8_t M_BLT  = 25;
constexpr uint8_t M_BLTE = 26;
constexpr uint8_t M_BE   = 27;
constexpr uint8_t M_BNE  = 28;
constexpr uint8_t M_BGT  = 29;
constexpr uint8_t M_BGTE = 30;
constexpr uint8_t M_MOVA = 31;
constexpr uint8_t M_MOVB = 32;
constexpr uint8_t M_NOP  = 33;
constexpr uint8_t M_SETC = 34;
constexpr uint8_t M_CLC  = 35;
constexpr uint8_t M_OUT  = 36;

enum class Microcode {
    NOP,
    IMM_TO_REG,
    MOV_A,
    MOV_B,
    ADD,
    ADDC,
    SUB,
    SUBC,
    AND,
    OR,
    XOR,
    SHL,
    SHLC,
    NOT_A,
    NOT_B,
    CMP,
    ST_A,
    ST_B,
    STI,
    LD,
    B,
    OUTI
};

enum class Opcode : uint8_t {
    ADD,
    ADDI,
    ADDC,
    SUB,
    SUBI,
    SUBC,
    NOT,
    AND,
    ANDI,
    OR,
    ORI,
    XOR,
    XORI,
    SHL,
    SHLC,
    LD,
    LDI,
    LDA,
    ST,
    STA,
    STI,
    STIA,
    CMP,
    B,
    BC,
    BLT,
    BLTE,
    BE,
    BNE,
    BGT,
    BGTE,
    MOVA,
    MOVB,
    NOP,
    SETC,
    CLC,
    OUT
};

struct MicrocodeInstruction {
    Microcode microcode_;
    uint16_t microcode_value_;
    std::string mnemonic_;
    MicrocodeInstruction(Microcode microcode, uint16_t microcode_value,
                         std::string mnemonic)
        : microcode_(microcode), microcode_value_(microcode_value),
          mnemonic_(mnemonic) {}
};

struct Instruction {
    Opcode opcode_;
    uint16_t opcode_value_;
    std::string mnemonic_;
    uint8_t rom_size_;
    Instruction(Opcode opcode, uint16_t opcode_value, std::string mnemonic,
                uint8_t rom_size)
        : opcode_(opcode), opcode_value_(opcode_value), mnemonic_(mnemonic),
          rom_size_(rom_size) {}
};

Microcode get_microcode_from_value(uint16_t value);
std::string get_microcode_string_from_code(Microcode code);
uint8_t get_value_of_opcode(Opcode opcode);
Opcode get_opcode_for_value(uint8_t opcode);
Opcode get_opcode_of_instruction(const std::vector<std::string> &symbols);
std::string get_string_for_opcode(const Opcode opcode);
uint8_t get_num_bytes_for_instruction(const Opcode opcode);
std::vector<Instruction> get_instructions();

const MicrocodeInstruction MI_NOP        = {Microcode::NOP, U_NOP, "nop"};
const MicrocodeInstruction MI_IMM_TO_REG = {Microcode::IMM_TO_REG, U_IMM_TO_REG,
                                            "imm_to_reg"};
const MicrocodeInstruction MI_MOV_A      = {Microcode::MOV_A, U_MOV_A, "mov_a"};
const MicrocodeInstruction MI_MOV_B      = {Microcode::MOV_B, U_MOV_B, "mov_b"};
const MicrocodeInstruction MI_ADD        = {Microcode::ADD, U_ADD, "add"};
const MicrocodeInstruction MI_ADDC       = {Microcode::ADDC, U_ADDC, "addc"};
const MicrocodeInstruction MI_SUB        = {Microcode::SUB, U_SUB, "sub"};
const MicrocodeInstruction MI_SUBC       = {Microcode::SUBC, U_SUBC, "subc"};
const MicrocodeInstruction MI_AND        = {Microcode::AND, U_AND, "and"};
const MicrocodeInstruction MI_OR         = {Microcode::OR, U_OR, "or"};
const MicrocodeInstruction MI_XOR        = {Microcode::XOR, U_XOR, "xor"};
const MicrocodeInstruction MI_SHL        = {Microcode::SHL, U_SHL, "shl"};
const MicrocodeInstruction MI_SHLC       = {Microcode::SHLC, U_SHLC, "shlc"};
const MicrocodeInstruction MI_NOT_A      = {Microcode::NOT_A, U_NOT_A, "not_a"};
const MicrocodeInstruction MI_NOT_B      = {Microcode::NOT_B, U_NOT_B, "not_b"};
const MicrocodeInstruction MI_CMP        = {Microcode::CMP, U_CMP, "cmp"};
const MicrocodeInstruction MI_ST_A       = {Microcode::ST_A, U_ST_A, "st_a"};
const MicrocodeInstruction MI_ST_B       = {Microcode::ST_B, U_ST_B, "st_b"};
const MicrocodeInstruction MI_STI        = {Microcode::STI, U_STI, "sti"};
const MicrocodeInstruction MI_LD         = {Microcode::LD, U_LD, "ld"};
const MicrocodeInstruction MI_B          = {Microcode::B, U_B, "b"};
const MicrocodeInstruction MI_OUTI       = {Microcode::OUTI, U_OUTI, "outi"};

const Instruction I_ADD  = {Opcode::ADD, M_ADD, "add", 1};
const Instruction I_ADDI = {Opcode::ADDI, M_ADDI, "addi", 2};
const Instruction I_ADDC = {Opcode::ADDC, M_ADDC, "addc", 1};
const Instruction I_SUB  = {Opcode::SUB, M_SUB, "sub", 1};
const Instruction I_SUBI = {Opcode::SUBI, M_SUBI, "subi", 2};
const Instruction I_SUBC = {Opcode::SUBC, M_SUBC, "subc", 1};
const Instruction I_NOT  = {Opcode::NOT, M_NOT, "not", 1};
const Instruction I_AND  = {Opcode::AND, M_AND, "and", 1};
const Instruction I_ANDI = {Opcode::ANDI, M_ANDI, "andi", 2};
const Instruction I_OR   = {Opcode::OR, M_OR, "or", 1};
const Instruction I_ORI  = {Opcode::ORI, M_ORI, "ori", 2};
const Instruction I_XOR  = {Opcode::XOR, M_XOR, "xor", 1};
const Instruction I_XORI = {Opcode::XORI, M_XORI, "xori", 2};
const Instruction I_SHL  = {Opcode::SHL, M_SHL, "shl", 1};
const Instruction I_SHLC = {Opcode::SHLC, M_SHLC, "shlc", 1};
const Instruction I_LD   = {Opcode::LD, M_LD, "ld", 1};
const Instruction I_LDI  = {Opcode::LDI, M_LDI, "ldi", 2};
const Instruction I_LDA  = {Opcode::LDA, M_LDA, "lda", 3};
const Instruction I_ST   = {Opcode::ST, M_ST, "st", 1};
const Instruction I_STA  = {Opcode::STA, M_STA, "sta", 3};
const Instruction I_STI  = {Opcode::STI, M_STI, "sti", 2};
const Instruction I_STIA = {Opcode::STIA, M_STIA, "stia", 4};
const Instruction I_CMP  = {Opcode::CMP, M_CMP, "cmp", 1};
const Instruction I_B    = {Opcode::B, M_B, "b", 3};
const Instruction I_BC   = {Opcode::BC, M_BC, "bc", 3};
const Instruction I_BLT  = {Opcode::BLT, M_BLT, "blt", 3};
const Instruction I_BLTE = {Opcode::BLTE, M_BLTE, "blte", 3};
const Instruction I_BE   = {Opcode::BE, M_BE, "be", 3};
const Instruction I_BNE  = {Opcode::BNE, M_BNE, "bne", 3};
const Instruction I_BGT  = {Opcode::BGT, M_BGT, "bgt", 3};
const Instruction I_BGTE = {Opcode::BGTE, M_BGTE, "bgte", 3};
const Instruction I_MOVA = {Opcode::MOVA, M_MOVA, "mov", 1};
const Instruction I_MOVB = {Opcode::MOVB, M_MOVB, "mov", 1};
const Instruction I_NOP  = {Opcode::NOP, M_NOP, "nop", 1};
const Instruction I_SETC = {Opcode::SETC, M_SETC, "setc", 1};
const Instruction I_CLC  = {Opcode::CLC, M_CLC, "clc", 1};
const Instruction I_OUT  = {Opcode::OUT, M_OUT, "out", 3};

} // namespace cpu

#endif
