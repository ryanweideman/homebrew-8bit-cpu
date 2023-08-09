#ifndef INSTRUCTION_PARSER_H
#define INSTRUCTION_PARSER_H

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../cpu.h"
#include "define_table.h"
#include "label_table.h"

class InstructionParser {
  public:
    InstructionParser(const LabelTable &label_table,
                      const DefineTable &define_table);
    std::vector<std::vector<std::string>> get_instructions() const;
    std::string parse_instruction(
        const std::vector<std::string> &tokenized_instruction) const;

  private:
    const LabelTable label_table_;
    const DefineTable define_table_;

    void init_opcode_map();
    std::vector<std::string>
    replace_label_tokens_with_addresses(const std::vector<std::string> &tokens,
                                        const LabelTable &label_table) const;
    uint8_t parse_register_string(const std::string &reg) const;
    std::string byte_to_hex_string(const uint8_t val) const;
    std::string parse_basic_instruction(const uint8_t opcode) const;
    std::string
    parse_imm_type_instruction(const uint8_t opcode,
                               const std::vector<std::string> &tokens) const;
    std::string
    parse_reg_type_instruction(const uint8_t opcode,
                               const std::vector<std::string> &tokens) const;
    std::string parse_load_address_type_instruction(
        const uint8_t opcode, const std::vector<std::string> &tokens) const;
    std::string parse_store_address_type_instruction(
        const uint8_t opcode, const std::vector<std::string> &tokens) const;
    std::string parse_store_imm_address_type_instruction(
        const uint8_t opcode, const std::vector<std::string> &tokens) const;
    std::string
    parse_branch_type_instruction(const uint8_t opcode,
                                  const std::vector<std::string> &tokens) const;
    std::string
    parse_move_type_instruction(const std::vector<std::string> &tokens) const;
    std::string
    parse_out_instruction(const uint8_t opcode,
                          const std::vector<std::string> &tokens) const;
};

#endif