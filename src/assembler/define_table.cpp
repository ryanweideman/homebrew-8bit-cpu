#include "define_table.h"

void DefineTable::put(const std::string name, const std::string value) {
    table_.emplace(name, value);
}

std::string DefineTable::get(const std::string &name) const {
    return table_.at(name);
}

bool DefineTable::contains(const std::string &name) const {
    return table_.find(name) != table_.end();
}

void DefineTable::print() const {
    std::cout << "~~~ Define Table ~~~" << std::endl;
    for (auto const &[name, address] : table_) {
        std::cout << name << " : " << address << std::endl;
    }
    std::cout << std::endl;
}