#ifndef LABEL_TABLE_H
#define LABEL_TABLE_H

#include <iostream>
#include <string>
#include <unordered_map>

class LabelTable {
  public:
    void put(std::string, int);
    int get_address(const std::string &) const;
    bool contains(const std::string &name) const;
    void print() const;

  private:
    std::unordered_map<std::string, int> table_;
};

#endif