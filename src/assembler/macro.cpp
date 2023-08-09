#include "macro.h"

Macro::Macro(const std::string &name, const std::vector<std::string> &args,
             const std::vector<std::vector<std::string>> &instructions)
    : name_(name), args_(args), instructions_(instructions) {}

std::string Macro::get_name() const { return name_; }

std::vector<std::vector<std::string>> Macro::get_instructions() const {
    return instructions_;
}

std::vector<std::string> Macro::get_arguments() const { return args_; }
