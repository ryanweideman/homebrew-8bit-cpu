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

    const DecoderRom decoder_rom = decoder::generate_decode_logic();
    Emulator emulator(state, prog_rom, decoder_rom);

    // falling edge : opcode loaded into IR
    emulator.advance_one_clock_edge();
    ASSERT_EQ(1, emulator.get_current_state().instruction_register);

    // rising edge : PC and UC inc
    emulator.advance_one_clock_edge();
    ASSERT_EQ(1, emulator.get_current_state().program_counter);
    ASSERT_EQ(1, emulator.get_current_state().microcode_counter);

    // falling edge : A + B result stored in B
    emulator.advance_one_clock_edge();
    ASSERT_EQ(1, emulator.get_current_state().a_register);
    ASSERT_EQ(5, emulator.get_current_state().b_register);
    ASSERT_EQ(1, emulator.get_current_state().flag_register);

    // rising edge : UC reset
    emulator.advance_one_clock_edge();
    ASSERT_EQ(0, emulator.get_current_state().microcode_counter);
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

    const DecoderRom decoder_rom = decoder::generate_decode_logic();
    Emulator emulator(state, prog_rom, decoder_rom);
    emulator.advance_one_instruction();

    ASSERT_EQ(4, emulator.get_current_state().instruction_register);
    ASSERT_EQ(2, emulator.get_current_state().program_counter);
    ASSERT_EQ(0, emulator.get_current_state().microcode_counter);
    ASSERT_EQ(24, emulator.get_current_state().a_register);
    ASSERT_EQ(23, emulator.get_current_state().b_register);
    ASSERT_EQ(1, emulator.get_current_state().flag_register);
}

TEST(EmulatorTest, Sub) {
    const ProgramRom prog_rom     = assembler::assemble({"sub A"});
    const ProgramRom expected_rom = {0x0C};
    ASSERT_EQ(prog_rom, expected_rom);

    State state;
    state.instruction_register = 0;
    state.flag_register        = 0;
    state.microcode_counter    = 0;
    state.lower_register       = 0;
    state.upper_register       = 0;
    state.program_counter      = 0;
    state.a_register           = 7;
    state.b_register           = 5;

    const DecoderRom decoder_rom = decoder::generate_decode_logic();
    Emulator emulator(state, prog_rom, decoder_rom);
    emulator.advance_one_instruction();

    state.instruction_register = 0x0C;
    state.program_counter      = 1;
    state.microcode_counter    = 0;
    state.a_register           = 2;
    state.b_register           = 5;
    state.flag_register        = 0;

    ASSERT_EQ(0x0C, emulator.get_current_state().instruction_register);
    ASSERT_EQ(1, emulator.get_current_state().program_counter);
    ASSERT_EQ(0, emulator.get_current_state().microcode_counter);
    ASSERT_EQ(2, emulator.get_current_state().a_register);
    ASSERT_EQ(5, emulator.get_current_state().b_register);
    ASSERT_EQ(0, emulator.get_current_state().flag_register);
}