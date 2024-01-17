#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include <cpu/cpu.h>
#include <cpu/state.h>
#include <decoder/decoder.h>

#include "emulator.h"
#include "microcode_executor.h"
#include "renderer.h"

Emulator::Emulator(const State initial_state, const ProgramRom &prog_rom,
                   const DecoderRom &decoder_rom)
    : current_state_(initial_state), prog_rom_(prog_rom),
      decoder_rom_(decoder_rom), total_number_clock_cycles_(0),
      total_number_instructions_(0) {
    clock_ = 1;
    current_opcode_ =
        cpu::get_opcode_for_value(prog_rom[initial_state.program_counter] >> 2);
    current_opcode_address_ = initial_state.program_counter;
    program_size_in_bytes_  = static_cast<int>(prog_rom.size());
}

void Emulator::advance_one_clock_edge() {
    clock_ ^= 1;
    previous_state_ = current_state_;
    current_state_  = microcode_executor::get_next_state(
        current_state_, clock_, prog_rom_, decoder_rom_);

    total_number_clock_cycles_ += clock_ == 1 ? 1 : 0;
    if (current_state_.microcode_counter == 0 && clock_ == 1 &&
        current_state_.program_counter < prog_rom_.size()) {
        total_number_instructions_++;
        current_opcode_ = cpu::get_opcode_for_value(
            prog_rom_[current_state_.program_counter] >> 2);
        current_opcode_address_ = current_state_.program_counter;
    }
}

void Emulator::advance_one_microcode_instruction() {
    if (clock_ != 1) {
        throw std::logic_error(
            "Microcode execution always starts with clock high");
    }
    // falling edge
    advance_one_clock_edge();
    // rising edge
    advance_one_clock_edge();
}

void Emulator::advance_one_instruction() {
    if (current_state_.microcode_counter != 0) {
        throw std::logic_error("Current microcode counter must be zero to "
                               "advance one full instruction");
    }
    advance_one_microcode_instruction();
    while (current_state_.microcode_counter != 0) {
        advance_one_microcode_instruction();
    }
}

void Emulator::advance_n_instructions(int n) {
    if (current_state_.microcode_counter != 0) {
        throw std::logic_error("Current microcode counter must be zero to "
                               "advance one full instruction");
    }
    for (int i = 0; i < n; i++) {
        advance_one_instruction();
    }
}

State Emulator::get_current_state() { return current_state_; }

State Emulator::get_previous_state() { return previous_state_; }

int Emulator::get_clock_state() { return clock_; }

cpu::Opcode Emulator::get_current_opcode() { return current_opcode_; }

uint16_t Emulator::get_current_opcode_address() {
    return current_opcode_address_;
}

float Emulator::get_clock_cycles_per_instruction() {
    return static_cast<float>(total_number_clock_cycles_) /
           total_number_instructions_;
}

int Emulator::get_total_number_clock_cycles() {
    return total_number_clock_cycles_;
}

int Emulator::get_total_number_instructions() {
    return total_number_instructions_;
}
