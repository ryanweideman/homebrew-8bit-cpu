#include <iomanip>
#include <iostream>
#include <sstream>

#include "instruction_serializer.h"

namespace instruction_serializer {

using std::vector;

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
vector<uint8_t> serialize_basic_instruction(const uint8_t opcode) {
    const uint8_t instruction_reg = opcode << 2;
    return {instruction_reg};
}

// format: opcode rd imm
vector<uint8_t>
serialize_imm_type_instruction(const uint8_t opcode,
                               const std::vector<std::string> &tokens) {
    const uint8_t destination_reg = serialize_register_string(tokens[1]);
    const uint8_t immediate_val   = std::stoi(tokens[2]);
    const uint8_t instruction_reg = (opcode << 2) | destination_reg;

    return {instruction_reg, immediate_val};
}

// format: opcode rd
vector<uint8_t>
serialize_reg_type_instruction(const uint8_t opcode,
                               const std::vector<std::string> &tokens) {
    const uint8_t destination_reg = serialize_register_string(tokens[1]);
    const uint8_t instruction_reg = (opcode << 2) | destination_reg;

    return {instruction_reg};
}

// format: opcode rd address
vector<uint8_t> serialize_load_address_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) {
    const uint8_t destination_reg   = serialize_register_string(tokens[1]);
    const uint16_t address          = std::stoi(tokens[2]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    const uint8_t instruction_reg   = (opcode << 2) | destination_reg;

    return {instruction_reg, lower_address_reg, upper_address_reg};
}

// format: opcode rs address
vector<uint8_t> serialize_store_address_type_instruction(
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

    return {instruction_reg, lower_address_reg, upper_address_reg};
}

// format opcode address imm
vector<uint8_t> serialize_store_imm_address_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) {
    const uint8_t immediate_val     = std::stoi(tokens[1]);
    const uint16_t address          = std::stoi(tokens[2]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    const uint8_t instruction_reg   = opcode << 2;

    return {instruction_reg, immediate_val, lower_address_reg,
            upper_address_reg};
}

// format: opcode addr
vector<uint8_t>
serialize_branch_type_instruction(const uint8_t opcode,
                                  const std::vector<std::string> &tokens) {
    const uint8_t instruction_reg   = opcode << 2;
    const uint16_t address          = std::stoi(tokens[1]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    return {instruction_reg, lower_address_reg, upper_address_reg};
}

// format: opcode rs rd
vector<uint8_t>
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

    return {instruction_reg};
}

vector<uint8_t>
serialize_out_instruction(const uint8_t opcode,
                          const std::vector<std::string> &tokens) {
    const uint8_t port            = std::stoi(tokens[1]);
    const uint8_t data            = std::stoi(tokens[2]);
    const uint8_t instruction_reg = (opcode << 2);

    return {instruction_reg, port, data};
}

std::string deserialize_register(const uint8_t rom_byte) {
    return "r" + std::to_string(rom_byte & 0x03);
}

std::string deserialize_imm_value(const uint8_t rom_byte) {
    return "x" + deserialize_byte(rom_byte);
}

std::string deserialize_byte(const uint8_t rom_byte) {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << std::right << std::hex
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

vector<uint8_t> serialize_instruction(const vector<std::string> &symbols) {

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

std::vector<std::string>
deserialize_instruction(const std::vector<uint8_t> bytes) {

    const cpu::Opcode opcode        = cpu::get_opcode_for_value(bytes[0] >> 2);
    const std::string opcode_string = cpu::get_string_for_opcode(opcode);

    switch (opcode) {
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
        return {opcode_string, deserialize_register(bytes[0])};

    case cpu::Opcode::ADDI:
    case cpu::Opcode::SUBI:
    case cpu::Opcode::ANDI:
    case cpu::Opcode::ORI:
    case cpu::Opcode::XORI:
    case cpu::Opcode::LDI:
    case cpu::Opcode::STI:
        return {opcode_string, deserialize_register(bytes[0]),
                deserialize_imm_value(bytes[1])};

    case cpu::Opcode::B:
    case cpu::Opcode::BC:
    case cpu::Opcode::BLT:
    case cpu::Opcode::BLTE:
    case cpu::Opcode::BE:
    case cpu::Opcode::BNE:
    case cpu::Opcode::BGT:
    case cpu::Opcode::BGTE:
        return {opcode_string, deserialize_address_value(bytes[1], bytes[2])};

    case cpu::Opcode::MOVA:
        return {opcode_string, "r0", deserialize_register(bytes[0])};
    case cpu::Opcode::MOVB:
        return {opcode_string, "r1", deserialize_register(bytes[0])};

    case cpu::Opcode::LDA:
    case cpu::Opcode::STA:
        return {opcode_string, deserialize_register(bytes[0]),
                deserialize_address_value(bytes[1], bytes[2])};

    case cpu::Opcode::STIA:
        return {opcode_string, deserialize_imm_value(bytes[1]),
                deserialize_address_value(bytes[2], bytes[3])};

    case cpu::Opcode::CMP:
    case cpu::Opcode::NOP:
    case cpu::Opcode::SETC:
    case cpu::Opcode::CLC:
        return {opcode_string};

    case cpu::Opcode::OUT:
        return {opcode_string, std::to_string(bytes[1]),
                deserialize_imm_value(bytes[2])};
    }

    throw std::invalid_argument(
        "Unknown opcode in instruction deserializing: " + opcode_string);
}
} // namespace instruction_serializer