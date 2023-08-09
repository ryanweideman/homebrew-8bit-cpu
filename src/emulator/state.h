#ifndef STATE_H
#define STATE_H

#include "../cpu.h"

struct State {
    uint8_t ram[cpu::RAM_SIZE];

    uint16_t program_counter;
    uint16_t microcode_counter;

    uint8_t instruction_register;
    uint8_t flag_register;
    uint8_t upper_register;
    uint8_t lower_register;
    uint8_t a_register;
    uint8_t b_register;
    uint8_t port_0;
    uint8_t port_1;

    State();
};

uint16_t get_microcode_bytes(
    State state, const std::array<uint16_t, cpu::DECODER_SIZE> &microcode_rom);

void print_state(State state);

#endif