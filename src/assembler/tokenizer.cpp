#include "tokenizer.h"

#include <sstream>
#include <string>
#include <vector>

namespace tokenizer {

std::vector<std::string> tokenize_line(const std::string &line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
}

} // namespace tokenizer