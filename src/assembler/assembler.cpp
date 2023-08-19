#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <cpu/cpu.h>
#include <cpu/instruction_serializer.h>

#include "assembler.h"
#include "define_table.h"
#include "label_table.h"
#include "macro.h"
#include "macro_generator.h"
#include "tokenizer.h"

namespace assembler {

std::vector<std::string> read_file(const std::string input_file_name) {
    std::ifstream in_file;
    in_file.open(input_file_name);
    if (in_file.fail()) {
        throw std::runtime_error("Could not open asm file with name: " +
                                 input_file_name);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in_file, line)) {
        lines.push_back(line);
    }

    std::cout << "Read asm file with path: " << input_file_name << std::endl;
    return lines;
}

void write_output_hex_file(const std::string output_file_name,
                           const std::vector<uint8_t> &bytes) {
    std::ofstream out_file;
    out_file.open(output_file_name);
    for (const uint8_t b : bytes) {
        std::stringstream stream;
        stream << std::setfill('0') << std::setw(2) << std::right << std::hex
               << static_cast<int>(b);
        out_file << stream.str() + " ";
    }
    out_file.close();
}

FirstPassResult first_pass(const std::vector<std::string> &input_lines) {
    LabelTable label_table;
    DefineTable define_table;
    MacroGenerator macro_generator;
    std::vector<std::vector<std::string>> symbol_table;

    int program_counter = 0;

    int line_index            = 0;
    const int number_of_lines = input_lines.size();
    while (line_index < number_of_lines) {
        if (program_counter >= 32768) {
            throw std::runtime_error("Max rom available is 32768 bytes, "
                                     "shorten the asm program");
        }

        const std::string line = input_lines[line_index];

        const TokenizedLine tokenized_line = tokenizer::tokenize_line(line);

        // Handle empty lines
        if (tokenized_line.empty()) {
            line_index++;
            continue;
        }

        // Handle comments
        if (tokenized_line[0].at(0) == '#') {
            line_index++;
            continue;
        }

        // Handle vars
        if (tokenized_line[0] == "%define") {
            const std::string define_name  = tokenized_line[1];
            const std::string define_value = tokenized_line[2];
            define_table.put(define_name, define_value);
            line_index++;
            continue;
        }

        // Handle Macro Definition
        if (tokenized_line[0] == "%macro") {
            line_index++;
            const std::string macro_definition_line = input_lines[line_index];
            const std::vector<std::string> macro_name_and_args =
                tokenizer::tokenize_line(macro_definition_line);
            const std::string macro_name = macro_name_and_args[0];
            const std::vector macro_args(macro_name_and_args.begin() + 1,
                                         macro_name_and_args.end());
            line_index++;
            std::vector<TokenizedLine> macro_instructions;
            // parse instructions until a .mend is reached
            while (line_index < number_of_lines) {
                std::string macro_line = input_lines[line_index];
                const TokenizedLine instruction =
                    tokenizer::tokenize_line(macro_line);
                // Handle empty lines and comments
                if (instruction.empty() || instruction[0].at(0) == '#') {
                    line_index++;
                    continue;
                }

                if (instruction[0] == "%endm") {
                    break;
                }

                macro_instructions.push_back(instruction);
                line_index++;
            }
            const Macro new_macro(macro_name, macro_args, macro_instructions);
            macro_generator.put(new_macro);
            line_index++;
            continue;
        }

        // Replace macro with instructions if found
        if (macro_generator.contains(tokenized_line.front())) {
            const std::string macro_name = tokenized_line.front();
            const std::vector<std::string> macro_args(
                tokenized_line.begin() + 1, tokenized_line.end());
            const auto macro_instructions =
                macro_generator.get_instructions(macro_name, macro_args);

            for (TokenizedLine macro_instruction : macro_instructions) {
                const std::string maybe_label = macro_instruction[0];
                if (maybe_label.back() == ':') {
                    const std::string label =
                        maybe_label.substr(0, maybe_label.size() - 1);
                    label_table.put(label, program_counter);
                    continue;
                }
                const cpu::Opcode opcode =
                    cpu::get_opcode_of_instruction(macro_instruction);
                program_counter += cpu::get_num_bytes_for_instruction(opcode);
                symbol_table.push_back(macro_instruction);
            }
            line_index++;
            continue;
        }

        // Handle Label
        std::string first_token = tokenized_line[0];
        if (first_token.back() == ':') {
            const std::string label =
                first_token.substr(0, first_token.size() - 1);
            label_table.put(label, program_counter);
            line_index++;
            continue;
        }

        cpu::Opcode opcode = cpu::get_opcode_of_instruction(tokenized_line);
        program_counter += cpu::get_num_bytes_for_instruction(opcode);

        // Store instruction
        symbol_table.push_back(tokenized_line);
        line_index++;
    }

    FirstPassResult first_pass_result;
    first_pass_result.symbol_table = symbol_table;
    first_pass_result.label_table  = label_table;
    first_pass_result.define_table = define_table;

    return first_pass_result;
}

std::vector<uint8_t>
second_pass(const std::vector<std::vector<std::string>> &symbol_table,
            const DefineTable &define_table, const LabelTable &label_table) {
    std::vector<uint8_t> bytes;
    for (const std::vector<std::string> &symbols : symbol_table) {
        // Use the label table and define table to translate symbols if needed
        std::vector<std::string> translated_symbols;
        for (const auto &symbol : symbols) {
            if (define_table.contains(symbol)) {
                translated_symbols.push_back(define_table.get(symbol));
            } else if (label_table.contains(symbol)) {
                translated_symbols.push_back(
                    std::to_string(label_table.get_address(symbol)));
            } else {
                translated_symbols.push_back(symbol);
            }
        }

        const std::vector<uint8_t> serialized_bytes =
            instruction_serializer::serialize_instruction(translated_symbols);
        for (const uint8_t b : serialized_bytes) {
            bytes.push_back(b);
        }
    }
    return bytes;
}

std::vector<uint8_t> assemble(const std::vector<std::string> &file_lines) {
    FirstPassResult first_pass_result = first_pass(file_lines);
    return second_pass(first_pass_result.symbol_table,
                       first_pass_result.define_table,
                       first_pass_result.label_table);
}

} // namespace assembler