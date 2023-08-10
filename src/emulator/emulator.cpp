#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "../cpu.h"
#include "../decoder/decoder.h"
#include "microcode_executor.h"
#include "state.h"

std::vector<std::string> tokenize_line(const std::string &line) {
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string word;
    while (ss >> word) {
        result.push_back(word);
    }
    return result;
}

std::vector<uint8_t> get_prog_rom(const std::string &hex_file_name) {
    std::ifstream in_file;
    in_file.open(hex_file_name);

    std::vector<uint8_t> prog_rom;
    if (in_file.is_open()) {
        std::string line;
        std::getline(in_file, line);
        const std::vector<std::string> tokenized_line = tokenize_line(line);
        for (const std::string token : tokenized_line) {
            prog_rom.push_back(static_cast<uint8_t>(std::stoi(token, 0, 16)));
        }
    }

    return prog_rom;
}

void print_instruction(State state, const std::vector<uint8_t> &prog_rom) {
    cpu::Opcode opcode =
        cpu::get_opcode_for_value(prog_rom[state.program_counter] >> 2);
    std::cout << "Instruction : " << cpu::get_string_for_opcode(opcode)
              << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Format: [input].hex\n");
        return 1;
    }
    const std::string hex_file_name = argv[1];

    const std::array<uint16_t, cpu::DECODER_SIZE> decoder_rom =
        decoder::generate_decode_logic();
    const std::vector<uint8_t> prog_rom = get_prog_rom(hex_file_name);

    State state;

    cpu::Opcode current_opcode =
        cpu::get_opcode_for_value(prog_rom[state.program_counter] >> 2);

    uint8_t clock = 0;
    for (int step = 0; step < 200; step++) {
        usleep(1000000);

        // Reset cursor
        std::cout << "\x1b[H\x1b[J" << std::endl;

        if (state.microcode_counter == 0 && clock == 0) {
            current_opcode =
                cpu::get_opcode_for_value(prog_rom[state.program_counter] >> 2);
        }

        State next_state = get_next_state(state, clock, prog_rom, decoder_rom);

        std::cout << "Instruction : "
                  << cpu::get_string_for_opcode(current_opcode) << std::endl;
        const uint16_t microcode = get_microcode_bytes(next_state, decoder_rom);
        const cpu::Microcode code = cpu::get_microcode_from_value(microcode);

        std::cout << "Microcode : " << cpu::get_microcode_string_from_code(code)
                  << std::endl;

        std::cout << "clock : " << std::to_string(clock) << std::endl;
        std::cout << std::endl;

        print_state(next_state);
        clock ^= 1;
        state = next_state;
    }

    return 0;
}