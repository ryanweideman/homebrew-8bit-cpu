#include "macro_generator.h"

void MacroGenerator::put(Macro macro) {
    table_.emplace(macro.get_name(), macro);
    count_table_.emplace(macro.get_name(), 1);
}

Macro MacroGenerator::get(const std::string &name) const {
    return table_.at(name);
}

bool MacroGenerator::contains(const std::string &name) const {
    return table_.find(name) != table_.end();
}

std::vector<std::vector<std::string>>
MacroGenerator::get_instructions(const std::string &macro_name,
                                 const std::vector<std::string> &args) {
    const Macro &macro = table_.at(macro_name);
    const std::vector<std::vector<std::string>> &untranslated_instructions =
        macro.get_instructions();
    const std::vector<std::string> &macroArgs = macro.get_arguments();

    const int count = count_table_.at(macro_name);
    count_table_.emplace(macro_name, count + 1);

    std::vector<std::vector<std::string>> translated_instructions;
    for (const std::vector<std::string> untranslated_instruction :
         untranslated_instructions) {
        std::vector<std::string> translated_instruction;
        for (const std::string token : untranslated_instruction) {
            std::string new_token;
            // Translate args
            for (size_t arg = 0; arg < args.size(); arg++) {
                if (token == macroArgs.at(arg)) {
                    new_token = args.at(arg);
                    break;
                }
            }
            // Replace "?" with count to support labels in macros
            if (new_token.empty()) {
                for (auto c : token) {
                    if (c == '?') {
                        new_token += "_" + std::to_string(count);
                    } else {
                        new_token += c;
                    }
                }
            }
            translated_instruction.push_back(new_token);
        }
        translated_instructions.push_back(translated_instruction);
    }

    return translated_instructions;
}

void MacroGenerator::print() const {
    std::cout << "~~~ Macro Table ~~~" << std::endl;
    if (table_.empty()) {
        std::cout << "no macros" << std::endl;
    }
    for (auto const &[name, macro] : table_) {
        std::cout << name << " ";
        if (macro.get_arguments().size() != 0) {
            for (auto const &token : macro.get_arguments()) {
                std::cout << token << " " << std::endl;
            }
        } else {
            std::cout << std::endl;
        }

        for (auto const &instruction : macro.get_instructions()) {
            std::cout << "  ";
            for (auto token : instruction) {
                std::cout << token << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}