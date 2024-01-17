#include <gtest/gtest.h>

#include <assembler/assembler.h>
#include <cpu/state.h>
#include <decoder/decoder.h>
#include <emulator/emulator.h>

TEST(EmulatorTest, Add) {
    const ProgramRom prog_rom     = assembler::assemble({"add B"});
    const ProgramRom expected_rom = {0x01};
    ASSERT_EQ(prog_rom, expected_rom);

    State state;
    state.instruction_register = 0;
    state.flag_register        = 0;
    state.microcode_counter    = 0;
    state.lower_register       = 0;
    state.upper_register       = 0;
    state.program_counter      = 0;
    state.a_register           = 1;
    state.b_register           = 4;

    Emulator emulator(state, prog_rom, decoder::generate_decode_logic());

    // falling edge : opcode loaded into IR
    emulator.advance_one_clock_edge();
    state.instruction_register = 1;
    ASSERT_EQ(state, emulator.get_current_state());

    // rising edge : PC and UC inc
    emulator.advance_one_clock_edge();
    state.program_counter   = 1;
    state.microcode_counter = 1;
    ASSERT_EQ(state, emulator.get_current_state());

    // falling edge : A + B result stored in B
    emulator.advance_one_clock_edge();
    state.a_register    = 1;
    state.b_register    = 5;
    state.flag_register = 1;
    ASSERT_EQ(state, emulator.get_current_state());

    // rising edge : UC reset
    emulator.advance_one_clock_edge();
    state.microcode_counter = 0;
    ASSERT_EQ(state, emulator.get_current_state());
}

TEST(EmulatorTest, Addi) {
    const ProgramRom prog_rom     = assembler::assemble({"addi A 23"});
    const ProgramRom expected_rom = {0x04, 0x17};
    ASSERT_EQ(prog_rom, expected_rom);

    State state;
    state.instruction_register = 0;
    state.flag_register        = 0;
    state.microcode_counter    = 0;
    state.lower_register       = 0;
    state.upper_register       = 0;
    state.program_counter      = 0;
    state.a_register           = 1;
    state.b_register           = 0;

    Emulator emulator(state, prog_rom, decoder::generate_decode_logic());
    emulator.advance_one_instruction();

    state.instruction_register = 4;
    state.program_counter      = 2;
    state.microcode_counter    = 0;
    state.a_register           = 24;
    state.b_register           = 23;
    state.flag_register        = 1;
    ASSERT_EQ(state, emulator.get_current_state());
}