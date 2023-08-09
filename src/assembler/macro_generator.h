#ifndef MACRO_GENERATOR_H
#define MACRO_GENERATOR_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "macro.h"

class MacroGenerator {
  public:
    void put(Macro);
    Macro get(const std::string &) const;
    std::vector<std::vector<std::string>>
    get_instructions(const std::string &, const std::vector<std::string> &);
    bool contains(const std::string &name) const;
    void print() const;

  private:
    std::unordered_map<std::string, Macro> table_;
    std::unordered_map<std::string, int> count_table_;
};

#endif