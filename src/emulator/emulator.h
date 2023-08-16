#ifndef EMULATOR_H
#define EMULATOR_H

#include <cpu/state.h>

typedef std::array<uint16_t, cpu::DECODER_SIZE> DecoderRom;
typedef std::vector<uint8_t> ProgramRom;

class Emulator {
  public:
    Emulator(const State initial_state, const ProgramRom &prog_rom,
             const DecoderRom &decoder_rom);

    void advance_one_clock_edge();

    State get_current_state();
    State get_previous_state();

    int get_clock_state();

    cpu::Opcode get_current_opcode();

    uint16_t get_current_opcode_address();

    float get_clock_cycles_per_instruction();

    int get_total_number_clock_cycles();

    int get_total_number_instructions();

  private:
    State current_state_;
    State previous_state_;

    const ProgramRom prog_rom_;
    const DecoderRom decoder_rom_;

    int clock_;

    cpu::Opcode current_opcode_;
    uint16_t current_opcode_address_;

    int total_number_clock_cycles_;
    int total_number_instructions_;
    int program_size_in_bytes_;
};

#endif
