#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "../cpu.h"
#include "microcode_executor.h"
#include "state.h"

State set_register_state(State current_state, uint8_t register_select,
                         uint8_t val) {
    State next_state = current_state;
    if (register_select == 0) {
        next_state.instruction_register = val;
    } else if (register_select == 1) {
        next_state.a_register = val;
    } else if (register_select == 2) {
        next_state.b_register = val;
    } else if (register_select == 3) {
        next_state.lower_register = val;
    } else if (register_select == 4) {
        next_state.upper_register = val;
    } else {
        exit(1);
    }
    return next_state;
}

State set_flag_state(const State current_state, const bool is_carry_set,
                     const bool is_zero_set) {
    State next_state         = current_state;
    next_state.flag_register = 0;
    next_state.flag_register |= is_carry_set ? 0 : 1;
    next_state.flag_register |= is_zero_set ? (1 << 1) : 0;
    return next_state;
}

State handle_rising_edge(State current_state, cpu::Microcode microcode,
                         uint8_t register_select, uint8_t microcode_reset,
                         const std::vector<uint8_t> &prog_rom) {
    State next_state = current_state;

    if (microcode_reset == 0) {
        next_state.microcode_counter = 0;
    } else {
        next_state.microcode_counter += 1;
    }

    switch (microcode) {
    case cpu::Microcode::OUTI:
    case cpu::Microcode::IMM_TO_REG:
    case cpu::Microcode::STI:
        next_state.program_counter += 1;
        break;
    case cpu::Microcode::B:
        next_state.program_counter =
            (current_state.upper_register << 8) | current_state.lower_register;
        break;
    case cpu::Microcode::LD:
    case cpu::Microcode::NOP:
    case cpu::Microcode::MOV_A:
    case cpu::Microcode::MOV_B:
    case cpu::Microcode::ADD:
    case cpu::Microcode::ADDC:
    case cpu::Microcode::SUB:
    case cpu::Microcode::SUBC:
    case cpu::Microcode::AND:
    case cpu::Microcode::OR:
    case cpu::Microcode::XOR:
    case cpu::Microcode::SHL:
    case cpu::Microcode::SHLC:
    case cpu::Microcode::NOT_A:
    case cpu::Microcode::NOT_B:
    case cpu::Microcode::CMP:
    case cpu::Microcode::ST_A:
    case cpu::Microcode::ST_B:
        break;
    }
    return next_state;
}

State handle_falling_edge(State current_state, cpu::Microcode microcode,
                          uint8_t register_select, uint8_t microcode_reset,
                          const std::vector<uint8_t> &prog_rom) {
    State next_state = current_state;

    switch (microcode) {
    case cpu::Microcode::NOP:
        break;
    case cpu::Microcode::IMM_TO_REG: {
        const uint8_t prog_rom_val = prog_rom[current_state.program_counter];
        next_state =
            set_register_state(next_state, register_select, prog_rom_val);
        break;
    }
    case cpu::Microcode::MOV_A: {
        next_state = set_register_state(next_state, register_select,
                                        next_state.a_register);
        break;
    }
    case cpu::Microcode::MOV_B: {
        next_state = set_register_state(next_state, register_select,
                                        next_state.b_register);
        break;
    }
    case cpu::Microcode::ADD: {
        // A + B
        uint16_t a_reg         = static_cast<uint16_t>(next_state.a_register);
        uint16_t b_reg         = static_cast<uint16_t>(next_state.b_register);
        uint16_t result        = a_reg + b_reg;
        uint8_t bounded_result = static_cast<uint8_t>(result & 0xFF);

        next_state =
            set_register_state(next_state, register_select, bounded_result);

        bool is_carry_set = ((result >> 8) & 1) == 1;
        bool is_zero_set  = (result & 0xFF) == 0;
        next_state = set_flag_state(next_state, is_carry_set, is_zero_set);
        break;
    }
    case cpu::Microcode::ADDC: {
        // A + B + 1
        uint16_t a_reg         = static_cast<uint16_t>(next_state.a_register);
        uint16_t b_reg         = static_cast<uint16_t>(next_state.b_register);
        uint16_t result        = a_reg + b_reg + 1;
        uint8_t bounded_result = static_cast<uint8_t>(result & 0xFF);

        next_state =
            set_register_state(next_state, register_select, bounded_result);

        bool is_carry_set = ((result >> 8) & 1) == 1;
        bool is_zero_set  = (result & 0xFF) == 0;
        next_state = set_flag_state(next_state, is_carry_set, is_zero_set);
        break;
    }

    case cpu::Microcode::SUB: {
        // A - B
        uint16_t result = static_cast<uint16_t>(current_state.a_register) +
                          static_cast<uint8_t>(~current_state.b_register) + 1;
        const uint8_t bounded_result = static_cast<uint8_t>(result & 0xFF);
        next_state =
            set_register_state(next_state, register_select, bounded_result);

        bool is_carry_set = ((result >> 8) & 1) == 1;
        bool is_zero_set  = (result & 0xFF) == 0;
        next_state = set_flag_state(next_state, is_carry_set, is_zero_set);
        break;
    }

    case cpu::Microcode::SUBC: {
        // A - B - 1
        const uint16_t result =
            static_cast<uint16_t>(current_state.a_register) +
            static_cast<uint8_t>(~current_state.b_register);
        const uint8_t bounded_result = static_cast<uint8_t>(result & 0xFF);
        next_state =
            set_register_state(next_state, register_select, bounded_result);

        bool is_carry_set = ((result >> 8) & 1) == 1;
        bool is_zero_set  = (result & 0xFF) == 0;
        next_state = set_flag_state(next_state, is_carry_set, is_zero_set);
        break;
    }

    case cpu::Microcode::AND: {
        uint8_t result = current_state.a_register & current_state.b_register;
        next_state = set_register_state(next_state, register_select, result);
        next_state.flag_register = 3;
        break;
    }
    case cpu::Microcode::OR: {
        uint8_t result = current_state.a_register | current_state.b_register;
        next_state = set_register_state(next_state, register_select, result);
        next_state.flag_register = 3;
        break;
    }
    case cpu::Microcode::XOR: {
        uint8_t result = current_state.a_register ^ current_state.b_register;
        next_state = set_register_state(next_state, register_select, result);
        next_state.flag_register = 3;
        break;
    }
    case cpu::Microcode::SHL: {
        uint8_t result = current_state.a_register << 1;
        next_state = set_register_state(next_state, register_select, result);
        next_state.flag_register = 3;
        break;
    }
    case cpu::Microcode::SHLC: {
        uint8_t result =
            (current_state.a_register << 1) | (current_state.flag_register & 1);
        next_state = set_register_state(next_state, register_select, result);
        next_state.flag_register = 3;
        break;
    }
    case cpu::Microcode::NOT_A: {
        uint8_t result = ~current_state.a_register;
        next_state = set_register_state(next_state, register_select, result);
        next_state.flag_register = 3;
        break;
    }
    case cpu::Microcode::NOT_B: {
        uint8_t result = ~current_state.b_register;
        next_state = set_register_state(next_state, register_select, result);
        next_state.flag_register = 3;
        break;
    }
    case cpu::Microcode::CMP: {
        // Same as SUB (A - B), but the result of the SUB is not stored, only
        // the flags are updated
        uint16_t result = static_cast<uint16_t>(current_state.a_register) +
                          static_cast<uint8_t>(~current_state.b_register) + 1;

        bool is_carry_set = ((result >> 8) & 1) == 1;
        bool is_zero_set  = (result & 0xFF) == 0;
        next_state = set_flag_state(next_state, is_carry_set, is_zero_set);
        break;
    }
    case cpu::Microcode::ST_A: {
        next_state.ram[(current_state.upper_register << 8) |
                       current_state.lower_register] = current_state.a_register;
        next_state.flag_register                     = 3;
        break;
    }
    case cpu::Microcode::ST_B: {
        next_state.ram[(current_state.upper_register << 8) |
                       current_state.lower_register] = current_state.b_register;
        next_state.flag_register                     = 3;
        break;
    }
    case cpu::Microcode::STI: {
        next_state.ram[(current_state.upper_register << 8) |
                       current_state.lower_register] =
            prog_rom[current_state.program_counter];
        break;
    }
    case cpu::Microcode::LD: {
        uint8_t data = current_state.ram[(current_state.upper_register << 8) |
                                         current_state.lower_register];
        next_state   = set_register_state(next_state, register_select, data);
        break;
    }
    case cpu::Microcode::B: {
        break;
    }
    case cpu::Microcode::OUTI: {
        const uint8_t prog_rom_val = prog_rom[current_state.program_counter];
        if (current_state.lower_register == 0) {
            next_state.port_0 = prog_rom_val;
        } else if (current_state.lower_register == 1) {
            next_state.port_1 = prog_rom_val;
        } else {
            std::cerr << "port " << std::to_string(prog_rom_val) << " not supported" << std::endl;
            exit(1);
        }
        break;
    }
    }

    return next_state;
}

std::string byte_to_hex_string(const uint8_t val) {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << std::right << std::hex
           << static_cast<int>(val);
    return stream.str();
}

State get_next_state(
    State current_state, uint8_t clock, const std::vector<uint8_t> &prog_rom,
    const std::array<uint16_t, cpu::DECODER_SIZE> &decoder_rom) {
    const uint16_t microcode = get_microcode_bytes(current_state, decoder_rom);
    const cpu::Microcode code = cpu::get_microcode_from_value(microcode);

    std::cout << "Microcode : " << cpu::get_microcode_string_from_code(code)
              << std::endl;
    uint8_t register_select = ((microcode & 0x80) >> 7) |
                              ((microcode & 0x40) >> 5) |
                              ((microcode & 0x20) >> 3);
    uint8_t microcode_reset = (microcode & 0x100) >> 8;

    // A single microcode instruction starts with the falling edge, and ends with the
    // the rising edge
    const State next_state =
        clock == 1 ? handle_rising_edge(current_state, code, register_select,
                                        microcode_reset, prog_rom)
                   : handle_falling_edge(current_state, code, register_select,
                                         microcode_reset, prog_rom);

    return next_state;
}
