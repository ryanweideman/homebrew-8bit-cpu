#include <iomanip>
#include <iostream>
#include <sstream>

#include "../cpu.h"
#include "../instruction_serializer.h"
#include "renderer.h"

namespace renderer {

void reset_screen() {
    // clears screen
    std::cout << "\033[2J";

    // moves cursor to (0, 0)
    std::cout << "\033[H";
}

void hide_cursor() { std::cout << "\e[?25l"; }

void move_cursor(const int row, const int col) {
    std::cout << "\033[" << std::to_string(row) << ";" << std::to_string(col)
              << "H";
}

std::string highlight_string(const std::string &input) {
    return "\x1b[48;5;22m" + input + "\x1b[0m";
}

void render_text_box(const int row, const int col,
                     const std::vector<std::string> &text) {
    for (size_t line = 0; line < text.size(); line++) {
        move_cursor(row + line, col);
        std::cout << text.at(line) << std::endl;
    }
}

void render_clock(const int row, const int col, const int clock) {
    render_text_box(row, col, {"~~~ Clock ~~~", std::to_string(clock)});
}

void render_counters(const int row, const int col, const State previous_state,
                     const State current_state) {
    const std::string program_counter_string =
        "program_counter : " + std::to_string(current_state.program_counter);
    const std::string microcode_counter_string =
        "microcode_counter : " +
        std::to_string(current_state.microcode_counter);

    render_text_box(
        row, col,
        {
            "~~~ Counters ~~~",
            current_state.program_counter != previous_state.program_counter
                ? highlight_string(program_counter_string)
                : program_counter_string,
            current_state.microcode_counter != previous_state.microcode_counter
                ? highlight_string(microcode_counter_string)
                : microcode_counter_string,
        });
}

void render_registers(const int row, const int col, const State previous_state,
                      const State current_state) {
    const std::string inst_reg_string =
        "instruction : " + std::to_string(current_state.instruction_register);
    const std::string flag_reg_string =
        "flags : " + std::to_string(current_state.flag_register);
    const std::string upper_reg_string =
        "upper : " + std::to_string(current_state.upper_register);
    const std::string lower_reg_string =
        "lower : " + std::to_string(current_state.lower_register);
    const std::string a_reg_string =
        "a : " + std::to_string(current_state.a_register);
    const std::string b_reg_string =
        "b : " + std::to_string(current_state.b_register);

    render_text_box(
        row, col,
        {"~~~ Registers ~~~",
         current_state.instruction_register !=
                 previous_state.instruction_register
             ? highlight_string(inst_reg_string)
             : inst_reg_string,
         current_state.flag_register != previous_state.flag_register
             ? highlight_string(flag_reg_string)
             : flag_reg_string,
         current_state.upper_register != previous_state.upper_register
             ? highlight_string(upper_reg_string)
             : upper_reg_string,
         current_state.lower_register != previous_state.lower_register
             ? highlight_string(lower_reg_string)
             : lower_reg_string,
         current_state.a_register != previous_state.a_register
             ? highlight_string(a_reg_string)
             : a_reg_string,
         current_state.b_register != previous_state.b_register
             ? highlight_string(b_reg_string)
             : b_reg_string});
}

void render_ports(const int row, const int col, const State previous_state,
                  const State current_state) {
    const std::string port_0_string =
        "0 : " + std::to_string(current_state.port_0);
    const std::string port_1_string =
        "1 : " + std::to_string(current_state.port_1);

    render_text_box(row, col,
                    {"~~~ Ports ~~~",
                     (current_state.port_0 != previous_state.port_0
                          ? highlight_string(port_0_string)
                          : port_0_string),

                     (current_state.port_1 != previous_state.port_1
                          ? highlight_string(port_1_string)
                          : port_1_string)});
}

void render_microcode(
    const int row, const int col, const State current_state,
    const std::array<uint16_t, cpu::DECODER_SIZE> &decoder_rom) {
    std::vector<std::string> microcode = {"~~~ Microcode ~~~"};
    for (int i = 0; i < 16; i++) {
        uint16_t microcode_address =
            ((current_state.flag_register << 4) |
             (current_state.instruction_register << 7)) +
            i;

        uint16_t microcode_value = decoder_rom[microcode_address];

        if (microcode_value == 0) {
            break;
        }

        const cpu::Microcode code =
            cpu::get_microcode_from_value(microcode_value);
        const std::string microcode_string =
            cpu::get_microcode_string_from_code(code);

        if (current_state.microcode_counter == i) {
            microcode.push_back("\x1b[48;5;32m" + microcode_string + "\x1b[0m");
        } else {
            microcode.push_back(microcode_string);
        }
    }
    render_text_box(row, col, microcode);
}

void render_instructions(const int row, const int col,
                         const cpu::Opcode current_opcode,
                         const uint16_t current_opcode_address,
                         const State current_state, const int clock,
                         const std::vector<uint8_t> &prog_rom) {

    cpu::Opcode next_opcode               = current_opcode;
    std::vector<std::string> instructions = {"~~~ Instructions ~~~"};

    int addr_width = 0;
    if (prog_rom.size() < 10) {
        addr_width = 1;
    } else if (prog_rom.size() < 100) {
        addr_width = 2;
    } else if (prog_rom.size() < 1000) {
        addr_width = 3;
    } else if (prog_rom.size() < 10000) {
        addr_width = 4;
    } else if (prog_rom.size() < 100000) {
        addr_width = 5;
    }

    std::stringstream stream;
    stream << std::setw(addr_width) << std::right
           << static_cast<int>(current_opcode_address);

    instructions.push_back(stream.str() + " : \x1b[48;5;32m" +
                           cpu::get_string_for_opcode(next_opcode) + "\x1b[0m");
    uint16_t current_offset = 0;
    for (int i = 0; i < 10; i++) {
        current_offset += get_num_bytes_for_instruction(next_opcode);
        if (current_opcode_address + current_offset >= prog_rom.size()) {
            break;
        }
        next_opcode = cpu::get_opcode_for_value(
            prog_rom[current_opcode_address + current_offset] >> 2);

        std::stringstream stream;
        stream << std::setw(addr_width) << std::right
               << static_cast<int>(current_opcode_address + current_offset);

        instructions.push_back(stream.str() + " : " +
                               instruction_serializer::deserialize_instruction(
                                   next_opcode,
                                   current_opcode_address + current_offset,
                                   prog_rom));
    }
    render_text_box(row, col, instructions);
}

void render_footer(const int row, const int col, const float cpi,
                   const int total_number_clock_cycles,
                   const int total_number_instructions,
                   const int prog_rom_size) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << cpi;
    std::string cpi_string = stream.str();
    render_text_box(row, col,
                    {
                        "CPI : " + cpi_string,
                        "total_number_clock_cycles : " +
                            std::to_string(total_number_clock_cycles),
                        "total_number_instructions : " +
                            std::to_string(total_number_instructions),
                        "program_size : " + std::to_string(prog_rom_size),
                    });
}

void render(const State previous_state, const State current_state,
            const cpu::Opcode current_opcode,
            const uint16_t current_opcode_address, const int clock,
            const float cpi, const int total_number_clock_cycles,
            const int total_number_instructions,
            const std::vector<uint8_t> &prog_rom,
            const std::array<uint16_t, cpu::DECODER_SIZE> &decoder_rom) {
    reset_screen();
    hide_cursor();

    render_text_box(1, 0, {"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"});
    render_text_box(2, 0, {"~~~~~~~~~~~~~~~~~ EMULATOR ~~~~~~~~~~~~~~~~~"});
    render_text_box(3, 0, {"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"});

    render_instructions(5, 2, current_opcode, current_opcode_address,
                        current_state, clock, prog_rom);
    render_microcode(18, 2, current_state, decoder_rom);

    render_clock(5, 30, clock);
    render_counters(8, 30, previous_state, current_state);
    render_registers(12, 30, previous_state, current_state);
    render_ports(20, 30, previous_state, current_state);

    render_text_box(24, 0, {"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"});
    render_footer(25, 0, cpi, total_number_clock_cycles,
                  total_number_instructions, static_cast<int>(prog_rom.size()));
}
} // namespace renderer