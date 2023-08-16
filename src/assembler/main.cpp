#include <string>
#include <vector>

#include "assembler.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Format: asm [input].asm [output]" << std::endl;
        return 1;
    }

    std::string input_file_name  = argv[1];
    std::string output_file_name = argv[2];

    std::vector<std::string> file_lines = read_file(input_file_name);
    std::vector<std::string> bytes      = assemble(file_lines);

    write_output_hex_file(output_file_name, bytes);

    return 0;
}