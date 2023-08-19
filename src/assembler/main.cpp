#include <string>
#include <vector>

#include "assembler.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Format: asm [input].asm [output]" << std::endl;
        return 1;
    }

    const std::string input_file_name  = argv[1];
    const std::string output_file_name = argv[2];

    const std::vector<std::string> file_lines =
        assembler::read_file(input_file_name);
    const std::vector<uint8_t> bytes = assembler::assemble(file_lines);

    assembler::write_output_hex_file(output_file_name, bytes);

    return 0;
}