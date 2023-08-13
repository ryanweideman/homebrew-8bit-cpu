#ifndef INSTRUCTION_PARSER_H
#define INSTRUCTION_PARSER_H

#include <string>
#include <vector>

#include "cpu.h"

namespace instruction_serializer {

std::string
serialize_instruction(const std::vector<std::string> &tokenized_instruction);

std::string deserialize_instruction(const cpu::Opcode current_opcode,
                                    const uint16_t address,
                                    const std::vector<uint8_t> &prog_rom);

} // namespace instruction_serializer

#endif