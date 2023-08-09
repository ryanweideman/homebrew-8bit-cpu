#ifndef MACRO_H
#define MACRO_H

#include <string>
#include <vector>

class Macro {
  public:
    Macro(const std::string &, const std::vector<std::string> &args,
          const std::vector<std::vector<std::string>> &instructions);
    std::string get_name() const;
    std::vector<std::vector<std::string>> get_instructions() const;
    std::vector<std::string> get_arguments() const;

  private:
    std::string name_;
    std::vector<std::string> args_;
    std::vector<std::vector<std::string>> instructions_;
};

#endif