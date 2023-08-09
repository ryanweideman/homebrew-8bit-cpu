#include "cpu.h"

namespace cpu {

const std::vector<MicrocodeInstruction> microcode_instruction_list = {
    MI_NOP,  MI_IMM_TO_REG, MI_MOV_A, MI_MOV_B, MI_ADD,  MI_ADDC,
    MI_SUB,  MI_SUBC,       MI_AND,   MI_OR,    MI_XOR,  MI_SHL,
    MI_SHLC, MI_NOT_A,      MI_NOT_B, MI_CMP,   MI_ST_A, MI_ST_B,
    MI_STI,  MI_LD,         MI_B,     MI_OUTI};

const std::vector<Instruction> instruction_list = {
    I_ADD, I_ADDI, I_ADDC, I_SUB,  I_SUBI, I_SUBC, I_NOT, I_AND,  I_ANDI,
    I_OR,  I_ORI,  I_XOR,  I_XORI, I_SHL,  I_SHLC, I_LD,  I_LDI,  I_LDA,
    I_ST,  I_STA,  I_STI,  I_STIA, I_CMP,  I_B,    I_BLT, I_BLTE, I_BE,
    I_BNE, I_BGT,  I_MOVA, I_MOVB, I_NOP,  I_SETC, I_CLC, I_OUT};

Microcode get_microcode_from_value(uint16_t value) {
    const uint16_t masked_value = value & U_IGNORE_REG_MASK;

    for (const MicrocodeInstruction &microcode_instruction :
         cpu::microcode_instruction_list) {
        if (masked_value ==
            (microcode_instruction.microcode_value_ & U_IGNORE_REG_MASK)) {
            return microcode_instruction.microcode_;
        }
    }
    std::cerr << "Failed to get microcode from string." << std::endl;
    exit(1);
}

std::string get_microcode_string_from_code(Microcode code) {
    for (const MicrocodeInstruction &microcode_instruction :
         cpu::microcode_instruction_list) {
        if (code == microcode_instruction.microcode_) {
            return microcode_instruction.mnemonic_;
        }
    }
    std::cerr << "Failed to get string of microcode." << std::endl;
    exit(1);
}

uint8_t get_value_of_opcode(Opcode opcode) {
    for (const Instruction &instruction : cpu::instruction_list) {
        if (opcode == instruction.opcode_) {
            return instruction.opcode_value_;
        }
    }
    std::cerr << "Failed to get value of opcode." << std::endl;
    exit(1);
}

Opcode get_opcode_for_value(uint8_t opcode) {
    for (const Instruction &instruction : cpu::instruction_list) {
        if (opcode == instruction.opcode_value_) {
            return instruction.opcode_;
        }
    }
    std::cerr << "Failed to get Opcode for value." << std::endl;
    exit(1);
}

Opcode get_opcode_for_string(const std::string opcode_string) {
    for (const Instruction &instruction : cpu::instruction_list) {
        if (opcode_string == instruction.mnemonic_) {
            return instruction.opcode_;
        }
    }
    std::cerr << "Failed to get Opcode for string: " << opcode_string
              << std::endl;
    exit(1);
}

std::string get_string_for_opcode(const Opcode opcode) {
    for (const Instruction &instruction : cpu::instruction_list) {
        if (opcode == instruction.opcode_) {
            return instruction.mnemonic_;
        }
    }
    std::cerr << "Failed to get string of opcode." << std::endl;
    exit(1);
}

uint8_t get_num_bytes_for_instruction(const Opcode opcode) {
    for (const Instruction &instruction : cpu::instruction_list) {
        if (opcode == instruction.opcode_) {
            return instruction.rom_size_;
        }
    }
    std::cerr << "Failed to get num bytes for opcode." << std::endl;
    exit(1);
}

std::vector<cpu::Instruction> get_instructions() { return instruction_list; }

} // namespace cpu