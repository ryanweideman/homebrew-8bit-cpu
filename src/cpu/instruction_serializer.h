#ifndef INSTRUCTION_PARSER_H
#define INSTRUCTION_PARSER_H

#include <string>
#include <vector>

#include "cpu.h"

namespace instruction_serializer {

std::vector<uint8_t>
serialize_instruction(const std::vector<std::string> &tokenized_instruction);

std::vector<std::string>
deserialize_instruction(const std::vector<uint8_t> bytes);

std::string deserialize_byte(const uint8_t rom_byte);

} // namespace instruction_serializer

#endif