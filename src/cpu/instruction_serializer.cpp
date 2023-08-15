#include <iomanip>
#include <iostream>
#include <sstream>

#include "instruction_serializer.h"

namespace instruction_serializer {

std::string byte_to_hex_string(const uint8_t val) {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << std::right << std::hex
           << static_cast<int>(val);
    return stream.str();
}

uint8_t serialize_register_string(const std::string &reg) {
    return std::stoi(reg.substr(1, 1));
}

// format: opcode
std::string serialize_basic_instruction(const uint8_t opcode) {
    const uint8_t instruction_reg = opcode << 2;
    return byte_to_hex_string(instruction_reg) + " ";
}

// format: opcode rd imm
std::string
serialize_imm_type_instruction(const uint8_t opcode,
                               const std::vector<std::string> &tokens) {
    const uint8_t destination_reg = serialize_register_string(tokens[1]);
    const uint8_t immediate_val   = std::stoi(tokens[2]);
    const uint8_t instruction_reg = (opcode << 2) | destination_reg;

    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(immediate_val) + " ";
}

// format: opcode rd
std::string
serialize_reg_type_instruction(const uint8_t opcode,
                               const std::vector<std::string> &tokens) {
    const uint8_t destination_reg = serialize_register_string(tokens[1]);
    const uint8_t instruction_reg = (opcode << 2) | destination_reg;

    return byte_to_hex_string(instruction_reg) + " ";
}

// format: opcode rd address
std::string serialize_load_address_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) {
    const uint8_t destination_reg   = serialize_register_string(tokens[1]);
    const uint16_t address          = std::stoi(tokens[2]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    const uint8_t instruction_reg   = (opcode << 2) | destination_reg;

    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(lower_address_reg) + " " +
           byte_to_hex_string(upper_address_reg) + " ";
}

// format: opcode rs address
std::string serialize_store_address_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) {
    const uint8_t source_reg = serialize_register_string(tokens[1]);

    if (source_reg > 1) {
        throw std::invalid_argument("Cannot store register " +
                                    std::string(tokens[1]));
    }

    const uint16_t address          = std::stoi(tokens[2]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    const uint8_t instruction_reg   = (opcode << 2) | source_reg;

    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(lower_address_reg) + " " +
           byte_to_hex_string(upper_address_reg) + " ";
}

// format opcode address imm
std::string serialize_store_imm_address_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) {
    const uint8_t immediate_val     = std::stoi(tokens[1]);
    const uint16_t address          = std::stoi(tokens[2]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    const uint8_t instruction_reg   = opcode << 2;

    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(immediate_val) + " " +
           byte_to_hex_string(lower_address_reg) + " " +
           byte_to_hex_string(upper_address_reg) + " ";
}

// format: opcode addr
std::string
serialize_branch_type_instruction(const uint8_t opcode,
                                  const std::vector<std::string> &tokens) {
    const uint8_t instruction_reg   = opcode << 2;
    const uint16_t address          = std::stoi(tokens[1]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(lower_address_reg) + " " +
           byte_to_hex_string(upper_address_reg) + " ";
}

// format: opcode rs rd
std::string
serialize_move_type_instruction(const std::vector<std::string> &tokens) {
    const uint8_t source_reg      = std::stoi(tokens[1].substr(1, 1));
    const uint8_t destination_reg = std::stoi(tokens[2].substr(1, 1));

    if (source_reg > 1) {
        throw std::invalid_argument("Cannot mov source register " +
                                    std::string(tokens[1]));
    }

    uint8_t instruction_reg;
    if (source_reg == 0) {
        instruction_reg = (cpu::M_MOVA << 2) | destination_reg;
    } else if (source_reg == 1) {
        instruction_reg = (cpu::M_MOVB << 2) | destination_reg;
    }

    return byte_to_hex_string(instruction_reg) + " ";
}

std::string serialize_out_instruction(const uint8_t opcode,
                                      const std::vector<std::string> &tokens) {
    const uint8_t port            = std::stoi(tokens[1]);
    const uint8_t data            = std::stoi(tokens[2]);
    const uint8_t instruction_reg = (opcode << 2);

    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(port) + " " + byte_to_hex_string(data) + " ";
}

std::string deserialize_register(const uint8_t rom_byte) {
    return "r" + std::to_string(rom_byte & 0x03);
}

std::string deserialize_imm_value(const uint8_t rom_byte) {
    std::stringstream stream;
    stream << "x" << std::setfill('0') << std::setw(2) << std::right << std::hex
           << static_cast<int>(rom_byte);
    return stream.str();
}

std::string deserialize_address_value(const uint8_t lower_byte,
                                      const uint8_t upper_byte) {
    const uint16_t address = (upper_byte << 8) | lower_byte;
    std::stringstream stream;
    stream << "x" << std::setfill('0') << std::setw(address <= 255 ? 2 : 4)
           << std::right << std::hex << static_cast<int>(address);
    return stream.str();
}

std::string serialize_instruction(const std::vector<std::string> &symbols) {

    const cpu::Opcode opcode = cpu::get_opcode_of_instruction(symbols);

    switch (opcode) {
    case cpu::Opcode::ADD:
        return serialize_reg_type_instruction(cpu::M_ADD, symbols);
    case cpu::Opcode::ADDI:
        return serialize_imm_type_instruction(cpu::M_ADDI, symbols);
    case cpu::Opcode::ADDC:
        return serialize_reg_type_instruction(cpu::M_ADDC, symbols);
    case cpu::Opcode::SUB:
        return serialize_reg_type_instruction(cpu::M_SUB, symbols);
    case cpu::Opcode::SUBI:
        return serialize_imm_type_instruction(cpu::M_SUBI, symbols);
    case cpu::Opcode::SUBC:
        return serialize_reg_type_instruction(cpu::M_SUBC, symbols);
    case cpu::Opcode::NOT:
        return serialize_reg_type_instruction(cpu::M_NOT, symbols);
    case cpu::Opcode::AND:
        return serialize_reg_type_instruction(cpu::M_AND, symbols);
    case cpu::Opcode::ANDI:
        return serialize_imm_type_instruction(cpu::M_ANDI, symbols);
    case cpu::Opcode::OR:
        return serialize_reg_type_instruction(cpu::M_OR, symbols);
    case cpu::Opcode::ORI:
        return serialize_imm_type_instruction(cpu::M_ORI, symbols);
    case cpu::Opcode::XOR:
        return serialize_reg_type_instruction(cpu::M_XOR, symbols);
    case cpu::Opcode::XORI:
        return serialize_imm_type_instruction(cpu::M_XORI, symbols);
    case cpu::Opcode::SHL:
        return serialize_reg_type_instruction(cpu::M_SHL, symbols);
    case cpu::Opcode::SHLC:
        return serialize_reg_type_instruction(cpu::M_SHLC, symbols);
    case cpu::Opcode::LD:
        return serialize_reg_type_instruction(cpu::M_LD, symbols);
    case cpu::Opcode::LDI:
        return serialize_imm_type_instruction(cpu::M_LDI, symbols);
    case cpu::Opcode::LDA:
        return serialize_load_address_type_instruction(cpu::M_LDA, symbols);
    case cpu::Opcode::ST:
        return serialize_reg_type_instruction(cpu::M_ST, symbols);
    case cpu::Opcode::STA:
        return serialize_store_address_type_instruction(cpu::M_STA, symbols);
    case cpu::Opcode::STI:
        return serialize_imm_type_instruction(cpu::M_STI, symbols);
    case cpu::Opcode::STIA:
        return serialize_store_imm_address_type_instruction(cpu::M_STIA,
                                                            symbols);
    case cpu::Opcode::CMP:
        return serialize_basic_instruction(cpu::M_CMP);
    case cpu::Opcode::B:
        return serialize_branch_type_instruction(cpu::M_B, symbols);
    case cpu::Opcode::BC:
        return serialize_branch_type_instruction(cpu::M_BC, symbols);
    case cpu::Opcode::BLT:
        return serialize_branch_type_instruction(cpu::M_BLT, symbols);
    case cpu::Opcode::BLTE:
        return serialize_branch_type_instruction(cpu::M_BLTE, symbols);
    case cpu::Opcode::BE:
        return serialize_branch_type_instruction(cpu::M_BE, symbols);
    case cpu::Opcode::BNE:
        return serialize_branch_type_instruction(cpu::M_BNE, symbols);
    case cpu::Opcode::BGT:
        return serialize_branch_type_instruction(cpu::M_BGT, symbols);
    case cpu::Opcode::BGTE:
        return serialize_branch_type_instruction(cpu::M_BGTE, symbols);
    case cpu::Opcode::MOVA:
        return serialize_move_type_instruction(symbols);
    case cpu::Opcode::MOVB:
        return serialize_move_type_instruction(symbols);
    case cpu::Opcode::NOP:
        return serialize_basic_instruction(cpu::M_NOP);
    case cpu::Opcode::SETC:
        return serialize_basic_instruction(cpu::M_SETC);
    case cpu::Opcode::CLC:
        return serialize_basic_instruction(cpu::M_CLC);
    case cpu::Opcode::OUT:
        return serialize_out_instruction(cpu::M_OUT, symbols);
    }

    throw std::invalid_argument("Unknown opcode in instruction serializing: " +
                                symbols[0]);
}

std::string deserialize_instruction(const cpu::Opcode current_opcode,
                                    const uint16_t address,
                                    const std::vector<uint8_t> &prog_rom) {
    std::stringstream stream;

    const std::string opcode_string =
        cpu::get_string_for_opcode(current_opcode);
    switch (current_opcode) {
    case cpu::Opcode::ADD:
    case cpu::Opcode::SUB:
    case cpu::Opcode::ADDC:
    case cpu::Opcode::SUBC:
    case cpu::Opcode::NOT:
    case cpu::Opcode::AND:
    case cpu::Opcode::OR:
    case cpu::Opcode::XOR:
    case cpu::Opcode::SHL:
    case cpu::Opcode::SHLC:
    case cpu::Opcode::LD:
    case cpu::Opcode::ST:
        stream << std::left << std::setw(5) << opcode_string << std::left
               << std::setw(3) << deserialize_register(prog_rom[address]);
        return stream.str();

    case cpu::Opcode::ADDI:
    case cpu::Opcode::SUBI:
    case cpu::Opcode::ANDI:
    case cpu::Opcode::ORI:
    case cpu::Opcode::XORI:
    case cpu::Opcode::LDI:
    case cpu::Opcode::STI:
    case cpu::Opcode::OUT:
        stream << std::left << std::setw(5) << opcode_string << std::left
               << std::setw(3) << deserialize_register(prog_rom[address]) << " "
               << deserialize_imm_value(prog_rom[address + 1]);
        return stream.str();

    case cpu::Opcode::B:
    case cpu::Opcode::BC:
    case cpu::Opcode::BLT:
    case cpu::Opcode::BLTE:
    case cpu::Opcode::BE:
    case cpu::Opcode::BNE:
    case cpu::Opcode::BGT:
    case cpu::Opcode::BGTE:
        stream << std::left << std::setw(5) << opcode_string << std::left
               << std::setw(3)
               << deserialize_address_value(prog_rom[address + 1],
                                            prog_rom[address + 2]);
        return stream.str();

    case cpu::Opcode::MOVA:
    case cpu::Opcode::MOVB:
        stream << std::left << std::setw(5) << opcode_string << std::left
               << std::setw(3) << deserialize_register(prog_rom[address]) << " "
               << deserialize_register(prog_rom[address + 1]);
        return stream.str();

    case cpu::Opcode::LDA:
    case cpu::Opcode::STA:
        stream << std::left << std::setw(5) << opcode_string << std::left
               << std::setw(3) << deserialize_register(prog_rom[address]) << " "
               << deserialize_address_value(prog_rom[address + 1],
                                            prog_rom[address + 2]);
        return stream.str();

    case cpu::Opcode::STIA:
        stream << std::left << std::setw(5) << opcode_string << std::left
               << std::setw(3) << deserialize_register(prog_rom[address + 1])
               << " " << deserialize_imm_value(prog_rom[address + 2]) << " "
               << deserialize_address_value(prog_rom[address + 3],
                                            prog_rom[address + 4]);
        return stream.str();

    case cpu::Opcode::CMP:
    case cpu::Opcode::NOP:
    case cpu::Opcode::SETC:
    case cpu::Opcode::CLC:
        return opcode_string;
    }

    throw std::invalid_argument(
        "Unknown opcode in instruction deserializing: " + opcode_string);
}
} // namespace instruction_serializer