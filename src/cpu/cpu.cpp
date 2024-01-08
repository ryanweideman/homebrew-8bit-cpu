#include "cpu.h"
#include <string>
#include <vector>

namespace cpu {

const std::vector<MicrocodeInstruction> microcode_instruction_list = {
    MI_NOP,  MI_IMM_TO_REG, MI_MOV_A, MI_MOV_B, MI_ADD,  MI_ADDC,
    MI_SUB,  MI_SUBC,       MI_AND,   MI_OR,    MI_XOR,  MI_SHL,
    MI_SHLC, MI_NOT_A,      MI_NOT_B, MI_CMP,   MI_ST_A, MI_ST_B,
    MI_STI,  MI_LD,         MI_B,     MI_OUTI};

const std::vector<Instruction> instruction_list = {
    I_ADD,  I_ADDI, I_ADDC, I_SUB, I_SUBI, I_SUBC, I_NOT,  I_AND, I_ANDI, I_OR,
    I_ORI,  I_XOR,  I_XORI, I_SHL, I_SHLC, I_LD,   I_LDI,  I_LDA, I_ST,   I_STA,
    I_STI,  I_STIA, I_CMP,  I_B,   I_BC,   I_BLT,  I_BLTE, I_BE,  I_BNE,  I_BGT,
    I_BGTE, I_MOVA, I_MOVB, I_NOP, I_SETC, I_CLC,  I_OUT};

Microcode get_microcode_from_value(uint16_t value) {
    const uint16_t masked_value = value & U_IGNORE_REG_MASK;

    for (const MicrocodeInstruction &microcode_instruction :
         microcode_instruction_list) {
        if (masked_value ==
            (microcode_instruction.microcode_value_ & U_IGNORE_REG_MASK)) {
            return microcode_instruction.microcode_;
        }
    }
    throw std::runtime_error("Failed to get microcode from string");
}

std::string get_microcode_string_from_code(Microcode code) {
    for (const MicrocodeInstruction &microcode_instruction :
         cpu::microcode_instruction_list) {
        if (code == microcode_instruction.microcode_) {
            return microcode_instruction.mnemonic_;
        }
    }
    throw std::runtime_error("Failed to get string of microcode");
}

uint8_t get_value_of_opcode(Opcode opcode) {
    for (const Instruction &instruction : instruction_list) {
        if (opcode == instruction.opcode_) {
            return instruction.opcode_value_;
        }
    }
    throw std::runtime_error("Failed to get value of opcode");
}

Opcode get_opcode_for_value(uint8_t opcode) {
    for (const Instruction &instruction : instruction_list) {
        if (opcode == instruction.opcode_value_) {
            return instruction.opcode_;
        }
    }
    throw std::runtime_error("Failed to get Opcode for value");
}

Opcode get_opcode_of_instruction(const std::vector<std::string> &symbols) {
    if (symbols[0] == "mov") {
        const std::string source_reg_string = symbols[1];
        uint8_t source_reg;
        if (source_reg_string == "A") {
            source_reg = 0;
        } else if (source_reg_string == "B") {
            source_reg = 1;
        } else if (source_reg_string == "L") {
            source_reg = 2;
        } else {
            source_reg = 3;
        }
        if (source_reg == 0) {
            return Opcode::MOVA;
        } else if (source_reg == 1) {
            return Opcode::MOVB;
        }
    }

    for (const Instruction &instruction : instruction_list) {
        if (symbols[0] == instruction.mnemonic_) {
            return instruction.opcode_;
        }
    }
    std::string error;
    for (const std::string &symbol : symbols) {
        error += symbol + " ";
    }
    throw std::runtime_error("Failed to get Opcode for line: " + error);
}

std::string get_string_for_opcode(const Opcode opcode) {
    for (const Instruction &instruction : instruction_list) {
        if (opcode == instruction.opcode_) {
            return instruction.mnemonic_;
        }
    }
    throw std::runtime_error("Failed to get string of opcode");
}

uint8_t get_num_bytes_for_instruction(const Opcode opcode) {
    for (const Instruction &instruction : instruction_list) {
        if (opcode == instruction.opcode_) {
            return instruction.rom_size_;
        }
    }
    throw std::runtime_error("Failed to get num bytes for opcode");
}

std::vector<cpu::Instruction> get_instructions() { return instruction_list; }

} // namespace cpu