#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "../cpu.h"
#include "../decoder/decoder.h"
#include "emulator.h"
#include "microcode_executor.h"
#include "renderer.h"
#include "state.h"

ProgramRom get_prog_rom(const std::string &hex_file_name) {
    std::ifstream in_file;
    in_file.open(hex_file_name);

    ProgramRom prog_rom;
    if (in_file.is_open()) {
        std::string line;
        std::getline(in_file, line);

        std::stringstream ss(line);
        std::string word;
        while (ss >> word) {
            prog_rom.push_back(static_cast<uint8_t>(std::stoi(word, 0, 16)));
        }
    }

    return prog_rom;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Format: [input].hex\n");
        return 1;
    }
    const std::string hex_file_name = argv[1];

    const DecoderRom decoder_rom = decoder::generate_decode_logic();
    const ProgramRom prog_rom    = get_prog_rom(hex_file_name);

    State initial_state;
    Emulator emulator(initial_state, prog_rom, decoder_rom);

    for (int step = 0; step < 1000; step++) {
        usleep(100000);

        emulator.advance_one_clock_edge();

        const State current_state        = emulator.get_current_state();
        const State previous_state       = emulator.get_previous_state();
        const int clock                  = emulator.get_clock_state();
        const cpu::Opcode current_opcode = emulator.get_current_opcode();
        const uint16_t current_opcode_address =
            emulator.get_current_opcode_address();
        const float cpi = emulator.get_clock_cycles_per_instruction();
        const int total_number_clock_cycles =
            emulator.get_total_number_clock_cycles();
        const int total_number_instructions =
            emulator.get_total_number_instructions();
        renderer::render(previous_state, current_state, current_opcode,
                         current_opcode_address, clock, cpi,
                         total_number_clock_cycles, total_number_instructions,
                         prog_rom, decoder_rom);
    }

    return 0;
}