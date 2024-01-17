#ifndef STATE_H
#define STATE_H

#include "cpu.h"

struct State {
    public: 
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

        bool operator==(const State &state) const {
            return program_counter == state.program_counter && 
                microcode_counter == state.microcode_counter && 
                instruction_register == state.instruction_register && 
                upper_register == state.upper_register && 
                lower_register == state.lower_register && 
                a_register == state.a_register && 
                b_register == state.b_register && 
                flag_register == state.flag_register && 
                port_0 == state.port_0 && 
                port_1 == state.port_1;
        };
};

void print_state(State state);

#endif