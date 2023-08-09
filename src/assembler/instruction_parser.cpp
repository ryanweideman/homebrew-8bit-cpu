#include "instruction_parser.h"

InstructionParser::InstructionParser(const LabelTable &label_table,
                                     const DefineTable &define_table)
    : label_table_(label_table), define_table_(define_table) {}

std::vector<std::string> InstructionParser::replace_label_tokens_with_addresses(
    const std::vector<std::string> &tokens,
    const LabelTable &label_table) const {
    std::vector<std::string> new_tokens;
    for (const auto &token : tokens) {
        if (label_table.contains(token)) {
            new_tokens.push_back(
                std::to_string(label_table.get_address(token)));
        } else {
            new_tokens.push_back(token);
        }
    }
    return new_tokens;
}

std::string InstructionParser::byte_to_hex_string(const uint8_t val) const {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << std::right << std::hex
           << static_cast<int>(val);
    return stream.str();
}

uint8_t InstructionParser::parse_register_string(const std::string &reg) const {
    return std::stoi(reg.substr(1, 1));
}

// format: opcode
std::string
InstructionParser::parse_basic_instruction(const uint8_t opcode) const {
    const uint8_t instruction_reg = opcode << 2;
    return byte_to_hex_string(instruction_reg) + " ";
}

// format: opcode rd imm
std::string InstructionParser::parse_imm_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) const {
    const uint8_t destination_reg = parse_register_string(tokens[1]);
    const uint8_t immediate_val   = std::stoi(tokens[2]);
    const uint8_t instruction_reg = (opcode << 2) | destination_reg;

    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(immediate_val) + " ";
}

// format: opcode rd
std::string InstructionParser::parse_reg_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) const {
    const uint8_t destination_reg = parse_register_string(tokens[1]);
    const uint8_t instruction_reg = (opcode << 2) | destination_reg;

    return byte_to_hex_string(instruction_reg) + " ";
}

// format: opcode rd address
std::string InstructionParser::parse_load_address_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) const {
    const uint8_t destination_reg   = parse_register_string(tokens[1]);
    const uint16_t address          = std::stoi(tokens[2]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    const uint8_t instruction_reg   = (opcode << 2) | destination_reg;

    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(lower_address_reg) + " " +
           byte_to_hex_string(upper_address_reg) + " ";
}

// format: opcode rs address
std::string InstructionParser::parse_store_address_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) const {
    const uint8_t source_reg = parse_register_string(tokens[1]);

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
std::string InstructionParser::parse_store_imm_address_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) const {
    const uint8_t immediate_val     = std::stoi(tokens[1]);
    const uint16_t address          = std::stoi(tokens[2]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    const uint8_t instruction_reg   = opcode << 2;

    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(lower_address_reg) + " " +
           byte_to_hex_string(upper_address_reg) + " " +
           byte_to_hex_string(immediate_val) + " ";
}

// format: opcode addr
std::string InstructionParser::parse_branch_type_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) const {
    const uint8_t instruction_reg   = opcode << 2;
    const uint16_t address          = std::stoi(tokens[1]);
    const uint8_t lower_address_reg = address & 0xFF;
    const uint8_t upper_address_reg = (address >> 8) & 0xFF;
    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(lower_address_reg) + " " +
           byte_to_hex_string(upper_address_reg) + " ";
}

// format: opcode rs rd
std::string InstructionParser::parse_move_type_instruction(
    const std::vector<std::string> &tokens) const {
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

std::string InstructionParser::parse_out_instruction(
    const uint8_t opcode, const std::vector<std::string> &tokens) const {
    const uint8_t port            = std::stoi(tokens[1]);
    const uint8_t data            = std::stoi(tokens[2]);
    const uint8_t instruction_reg = (opcode << 2);

    return byte_to_hex_string(instruction_reg) + " " +
           byte_to_hex_string(port) + " " + byte_to_hex_string(data) + " ";
}

std::string InstructionParser::parse_instruction(
    const std::vector<std::string> &symbols) const {

    cpu::Opcode opcode;
    if (symbols[0] == "mov") {
        const uint8_t source_reg = std::stoi(symbols[1].substr(1, 1));
        if (source_reg == 0) {
            opcode = cpu::Opcode::MOVA;
        } else if (source_reg == 1) {
            opcode = cpu::Opcode::MOVB;
        }
    } else {
        opcode = cpu::get_opcode_for_string(symbols[0]);
    }

    std::cout << cpu::get_string_for_opcode(opcode) << std::endl;

    switch (opcode) {
    case cpu::Opcode::ADD:
        return parse_reg_type_instruction(cpu::M_ADD, symbols);
    case cpu::Opcode::ADDI:
        return parse_imm_type_instruction(cpu::M_ADDI, symbols);
    case cpu::Opcode::ADDC:
        return parse_reg_type_instruction(cpu::M_ADDC, symbols);
    case cpu::Opcode::SUB:
        return parse_reg_type_instruction(cpu::M_SUB, symbols);
    case cpu::Opcode::SUBI:
        return parse_imm_type_instruction(cpu::M_SUBI, symbols);
    case cpu::Opcode::SUBC:
        return parse_reg_type_instruction(cpu::M_SUBC, symbols);
    case cpu::Opcode::NOT:
        return parse_reg_type_instruction(cpu::M_NOT, symbols);
    case cpu::Opcode::AND:
        return parse_reg_type_instruction(cpu::M_AND, symbols);
    case cpu::Opcode::ANDI:
        return parse_imm_type_instruction(cpu::M_ANDI, symbols);
    case cpu::Opcode::OR:
        return parse_reg_type_instruction(cpu::M_OR, symbols);
    case cpu::Opcode::ORI:
        return parse_imm_type_instruction(cpu::M_ORI, symbols);
    case cpu::Opcode::XOR:
        return parse_reg_type_instruction(cpu::M_XOR, symbols);
    case cpu::Opcode::XORI:
        return parse_imm_type_instruction(cpu::M_XORI, symbols);
    case cpu::Opcode::SHL:
        return parse_reg_type_instruction(cpu::M_SHL, symbols);
    case cpu::Opcode::SHLC:
        return parse_reg_type_instruction(cpu::M_SHLC, symbols);
    case cpu::Opcode::LD:
        return parse_reg_type_instruction(cpu::M_LD, symbols);
    case cpu::Opcode::LDI:
        return parse_imm_type_instruction(cpu::M_LDI, symbols);
    case cpu::Opcode::LDA:
        return parse_load_address_type_instruction(cpu::M_LDA, symbols);
    case cpu::Opcode::ST:
        return parse_reg_type_instruction(cpu::M_ST, symbols);
    case cpu::Opcode::STA:
        return parse_store_address_type_instruction(cpu::M_STA, symbols);
    case cpu::Opcode::STI:
        return parse_imm_type_instruction(cpu::M_STI, symbols);
    case cpu::Opcode::STIA:
        return parse_store_imm_address_type_instruction(cpu::M_STIA, symbols);
    case cpu::Opcode::CMP:
        return parse_basic_instruction(cpu::M_CMP);
    case cpu::Opcode::B:
        return parse_branch_type_instruction(cpu::M_B, symbols);
    case cpu::Opcode::BC:
        return parse_branch_type_instruction(cpu::M_BC, symbols);
    case cpu::Opcode::BLT:
        return parse_branch_type_instruction(cpu::M_BLT, symbols);
    case cpu::Opcode::BLTE:
        return parse_branch_type_instruction(cpu::M_BLTE, symbols);
    case cpu::Opcode::BE:
        return parse_branch_type_instruction(cpu::M_BE, symbols);
    case cpu::Opcode::BNE:
        return parse_branch_type_instruction(cpu::M_BNE, symbols);
    case cpu::Opcode::BGT:
        return parse_branch_type_instruction(cpu::M_BGT, symbols);
    case cpu::Opcode::BGTE:
        return parse_branch_type_instruction(cpu::M_BGTE, symbols);
    case cpu::Opcode::MOVA:
        return parse_move_type_instruction(symbols);
    case cpu::Opcode::MOVB:
        return parse_move_type_instruction(symbols);
    case cpu::Opcode::NOP:
        return parse_basic_instruction(cpu::M_NOP);
    case cpu::Opcode::SETC:
        return parse_basic_instruction(cpu::M_SETC);
    case cpu::Opcode::CLC:
        return parse_basic_instruction(cpu::M_CLC);
    case cpu::Opcode::OUT:
        return parse_out_instruction(cpu::M_OUT, symbols);
    }

    throw std::invalid_argument("Unknown opcode in instruction parsing: " +
                                std::string(symbols[0]));
}