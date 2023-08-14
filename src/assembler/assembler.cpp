#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <cpu/cpu.h>
#include <cpu/instruction_serializer.h>

#include "define_table.h"
#include "label_table.h"
#include "macro.h"
#include "macro_generator.h"
#include "tokenizer.h"

void assemble(const std::string input_file_name,
              const std::string output_file_name) {
    // Open input asm file
    std::ifstream in_file;
    in_file.open(input_file_name);
    if (in_file.fail()) {
        throw std::runtime_error("Could not open asm file with name: " +
                                 input_file_name);
    }
    std::cout << "Parsing asm file with path: " << input_file_name << std::endl;

    LabelTable label_table;
    DefineTable define_table;
    MacroGenerator macro_generator;
    std::vector<std::vector<std::string>> symbol_table;

    int program_counter = 0;

    if (in_file.is_open()) {
        std::string line;
        while (std::getline(in_file, line)) {
            if (program_counter >= 32768) {
                throw std::runtime_error("Max rom available is 32768 bytes, "
                                         "shorten the asm program");
            }

            const std::vector<std::string> tokenized_line =
                tokenizer::tokenize_line(line);

            // Handle empty lines
            if (tokenized_line.empty()) {
                continue;
            }

            // Handle comments
            if (tokenized_line[0].at(0) == '#') {
                continue;
            }

            // Handle vars
            if (tokenized_line[0] == "%define") {
                const std::string define_name  = tokenized_line[1];
                const std::string define_value = tokenized_line[2];
                define_table.put(define_name, define_value);
                continue;
            }

            // Handle Macro Definition
            if (tokenized_line[0] == "%macro") {
                std::cout << "parsing macro" << std::endl;
                if (!std::getline(in_file, line)) {
                    throw std::invalid_argument(
                        "File unexpectedly ended while processing macro");
                }
                const std::vector<std::string> macro_name_and_args =
                    tokenizer::tokenize_line(line);
                const std::string macro_name = macro_name_and_args[0];
                const std::vector macro_args(macro_name_and_args.begin() + 1,
                                             macro_name_and_args.end());
                std::vector<std::vector<std::string>> macro_instructions;
                // parse instructions until a .mend is reached
                while (std::getline(in_file, line)) {
                    const std::vector<std::string> instruction =
                        tokenizer::tokenize_line(line);
                    // Handle empty lines and comments
                    if (instruction.empty() || instruction[0].at(0) == '#') {
                        continue;
                    }

                    if (instruction[0] == "%endm") {
                        break;
                    }

                    macro_instructions.push_back(instruction);
                }
                const Macro new_macro(macro_name, macro_args,
                                      macro_instructions);
                macro_generator.put(new_macro);
                continue;
            }

            // Replace macro with instructions if found
            if (macro_generator.contains(tokenized_line.front())) {
                const std::string macro_name = tokenized_line.front();
                const std::vector<std::string> macro_args(
                    tokenized_line.begin() + 1, tokenized_line.end());
                const auto macro_instructions =
                    macro_generator.get_instructions(macro_name, macro_args);

                for (std::vector<std::string> macro_instruction :
                     macro_instructions) {
                    const std::string maybe_label = macro_instruction[0];
                    if (maybe_label.back() == ':') {
                        const std::string label =
                            maybe_label.substr(0, maybe_label.size() - 1);
                        label_table.put(label, program_counter);
                        continue;
                    }
                    const cpu::Opcode opcode =
                        cpu::get_opcode_of_instruction(macro_instruction);
                    program_counter +=
                        cpu::get_num_bytes_for_instruction(opcode);
                    symbol_table.push_back(macro_instruction);
                }
                continue;
            }

            // Handle Label
            std::string first_token = tokenized_line[0];
            if (first_token.back() == ':') {
                const std::string label =
                    first_token.substr(0, first_token.size() - 1);
                label_table.put(label, program_counter);
                continue;
            }

            cpu::Opcode opcode = cpu::get_opcode_of_instruction(tokenized_line);
            program_counter += cpu::get_num_bytes_for_instruction(opcode);

            // Store instruction
            symbol_table.push_back(tokenized_line);
        }
    }

    in_file.close();

    label_table.print();
    macro_generator.print();
    define_table.print();

    std::cout << "2nd pass, parsing instructions" << std::endl;
    std::ofstream out_file;
    out_file.open(output_file_name);
    for (const std::vector<std::string> &symbols : symbol_table) {
        // Use the label table and define table to translate symbols if needed
        std::vector<std::string> translated_symbols;
        for (const auto &symbol : symbols) {
            if (define_table.contains(symbol)) {
                translated_symbols.push_back(define_table.get(symbol));
                std::cout << symbol << " " << define_table.get(symbol)
                          << std::endl;
            } else if (label_table.contains(symbol)) {
                translated_symbols.push_back(
                    std::to_string(label_table.get_address(symbol)));
            } else {
                translated_symbols.push_back(symbol);
            }
        }

        const std::string opcode = translated_symbols[0];
        out_file << instruction_serializer::serialize_instruction(
            translated_symbols);
    }
    out_file.close();
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Format: asm [input].asm [output]" << std::endl;
        return 1;
    }

    std::string input_file_name  = argv[1];
    std::string output_file_name = argv[2];

    assemble(input_file_name, output_file_name);

    return 0;
}
