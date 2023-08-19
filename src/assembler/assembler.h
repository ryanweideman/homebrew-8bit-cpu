#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "define_table.h"
#include "label_table.h"

namespace assembler {

typedef std::vector<std::string> TokenizedLine;

struct FirstPassResult {
    std::vector<std::vector<std::string>> symbol_table;
    DefineTable define_table;
    LabelTable label_table;
};

std::vector<std::string> read_file(const std::string input_file_name);
void write_output_hex_file(const std::string output_file_name,
                           const std::vector<uint8_t> &bytes);

std::vector<uint8_t> assemble(const std::vector<std::string> &lines);

} // namespace assembler

#endif