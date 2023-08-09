#include "label_table.h"

void LabelTable::put(std::string name, int address) {
    table_.emplace(name, address);
}

int LabelTable::get_address(const std::string &name) const {
    return table_.at(name);
}

bool LabelTable::contains(const std::string &name) const {
    return table_.find(name) != table_.end();
}

void LabelTable::print() const {
    std::cout << "~~~ Label Table ~~~" << std::endl;
    for (auto const &[name, address] : table_) {
        std::cout << name << " : " << address << std::endl;
    }
    std::cout << std::endl;
}