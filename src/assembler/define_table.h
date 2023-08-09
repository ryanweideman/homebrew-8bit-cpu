#ifndef VARIABLE_TABLE_H
#define VARIABLE_TABLE_H

#include <iostream>
#include <string>
#include <unordered_map>

class DefineTable {
  public:
    void put(const std::string, const std::string);
    std::string get(const std::string &name) const;
    bool contains(const std::string &name) const;
    void print() const;

  private:
    std::unordered_map<std::string, std::string> table_;
};

#endif