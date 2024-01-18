#include <gtest/gtest.h>

#include <assembler/assembler.h>
#include <cpu/state.h>
#include <decoder/decoder.h>
#include <emulator/emulator.h>
#include <testing_commons/commons.h>

TEST(IntegrationTest, BasicAddition) {
    // clang-format off
    const ProgramRom prog_rom = assembler::assemble({
        "start:", 
        "  ldi A 5", 
        "  ldi B 7", 
        "  add A", 
        "end:", 
        "  b end"});
    // clang-format on
    const ProgramRom expected_rom = {0x3c, 0x05, 0x3d, 0x07,
                                     0x00, 0x5c, 0x05, 0x00};
    ASSERT_EQ(prog_rom, expected_rom);

    const DecoderRom decoder_rom = decoder::generate_decode_logic();

    State state;
    state.a_register      = 0;
    state.b_register      = 0;
    state.lower_register  = 0;
    state.upper_register  = 0;
    state.program_counter = 0;

    Emulator emulator(state, prog_rom, decoder_rom);

    // execute ldi A 5
    emulator.advance_one_instruction();
    state.instruction_register = 60;
    state.program_counter      = 2;
    state.a_register           = 5;
    ASSERT_TRUE(testing_commons::assert_state_equals(
        state, emulator.get_current_state()));

    // execute ldi B 7
    emulator.advance_one_instruction();
    state.instruction_register = 61;
    state.program_counter      = 4;
    state.b_register           = 7;
    ASSERT_TRUE(testing_commons::assert_state_equals(
        state, emulator.get_current_state()));

    // execute add A
    emulator.advance_one_instruction();
    state.instruction_register = 0;
    state.program_counter      = 5;
    state.a_register           = 12;
    state.flag_register        = 1;
    ASSERT_TRUE(testing_commons::assert_state_equals(
        state, emulator.get_current_state()));

    // execute b end
    emulator.advance_one_instruction();
    state.instruction_register = 92;
    state.lower_register       = 5;
    state.upper_register       = 0;
    ASSERT_TRUE(testing_commons::assert_state_equals(
        state, emulator.get_current_state()));

    // execute b end
    emulator.advance_one_instruction();
    state.instruction_register = 92;
    state.lower_register       = 5;
    state.upper_register       = 0;
    ASSERT_TRUE(testing_commons::assert_state_equals(
        state, emulator.get_current_state()));
}

TEST(IntegrationTest, Add_carry) {
    // clang-format off
    const ProgramRom prog_rom =
        assembler::assemble({
            "ldi A 128", 
            "ldi B 128", 
            "add A", 
            "ldi A 30",
            "ldi B 7", 
            "addc A"});
    // clang-format on

    const DecoderRom decoder_rom = decoder::generate_decode_logic();

    State initial_state;
    initial_state.a_register = 0;
    initial_state.b_register = 0;

    Emulator emulator(initial_state, prog_rom, decoder_rom);

    ASSERT_EQ(0, emulator.get_current_state().a_register);
    ASSERT_EQ(0, emulator.get_current_state().b_register);
    ASSERT_EQ(0, emulator.get_current_state().program_counter);

    emulator.advance_n_instructions(6);

    // 30 (a) + 7 (b) + 1 (carry) = 38
    ASSERT_EQ(38, emulator.get_current_state().a_register);
    ASSERT_EQ(7, emulator.get_current_state().b_register);
}