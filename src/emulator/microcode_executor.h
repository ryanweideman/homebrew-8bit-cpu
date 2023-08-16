#ifndef MICROCODE_EXECUTOR_H
#define MICROCODE_EXECUTOR_H

#include <cpu/state.h>

namespace microcode_executor {

State get_next_state(
    State current_state, uint8_t clock, const std::vector<uint8_t> &prog_rom,
    const std::array<uint16_t, cpu::DECODER_SIZE> &decoder_rom);

}

#endif