#ifndef RENDERER_H
#define RENDERER_H

#include <cpu/state.h>

namespace renderer {
void render(const State previous_state, const State current_state,
            const cpu::Opcode current_opcode,
            const uint16_t current_opcode_address, const int clock,
            const float cpi, const int total_number_clock_cycles,
            const int total_number_instructions,
            const std::vector<uint8_t> &prog_rom,
            const std::array<uint16_t, cpu::DECODER_SIZE> &decoder_rom);
}

#endif