#include <gtest/gtest.h>

#include <assembler/assembler.h>
#include <cpu/state.h>
#include <decoder/decoder.h>
#include <emulator/emulator.h>

TEST(IntegrationTest, Basic) {
    // clang-format off
    const ProgramRom prog_rom = assembler::assemble({
        "start:", 
        "  ldi r0 5", 
        "  ldi r1 7", 
        "  nop", 
        "end:", 
        "  b end"});
    // clang-format on
    const ProgramRom expected_rom = {0x3c, 0x05, 0x3d, 0x07,
                                     0x84, 0x5c, 0x05, 0x00};
    ASSERT_EQ(prog_rom, expected_rom);

    const DecoderRom decoder_rom = decoder::generate_decode_logic();

    State initial_state;
    initial_state.a_register     = 0;
    initial_state.b_register     = 0;
    initial_state.lower_register = 0;
    initial_state.upper_register = 0;

    Emulator emulator(initial_state, prog_rom, decoder_rom);

    ASSERT_EQ(0, emulator.get_current_state().a_register);
    ASSERT_EQ(0, emulator.get_current_state().b_register);
    ASSERT_EQ(0, emulator.get_current_state().lower_register);
    ASSERT_EQ(0, emulator.get_current_state().upper_register);
    ASSERT_EQ(0, emulator.get_current_state().program_counter);

    // execute ldi r0 5
    emulator.advance_one_instruction();
    ASSERT_EQ(5, emulator.get_current_state().a_register);
    ASSERT_EQ(0, emulator.get_current_state().b_register);
    ASSERT_EQ(0, emulator.get_current_state().lower_register);
    ASSERT_EQ(0, emulator.get_current_state().upper_register);
    ASSERT_EQ(2, emulator.get_current_state().program_counter);

    // execute ldi r1 7
    emulator.advance_one_instruction();
    ASSERT_EQ(5, emulator.get_current_state().a_register);
    ASSERT_EQ(7, emulator.get_current_state().b_register);
    ASSERT_EQ(0, emulator.get_current_state().lower_register);
    ASSERT_EQ(0, emulator.get_current_state().upper_register);
    ASSERT_EQ(4, emulator.get_current_state().program_counter);

    // execute nop
    emulator.advance_one_instruction();
    ASSERT_EQ(5, emulator.get_current_state().a_register);
    ASSERT_EQ(7, emulator.get_current_state().b_register);
    ASSERT_EQ(0, emulator.get_current_state().lower_register);
    ASSERT_EQ(0, emulator.get_current_state().upper_register);
    ASSERT_EQ(5, emulator.get_current_state().program_counter);

    // execute b end
    emulator.advance_one_instruction();
    ASSERT_EQ(5, emulator.get_current_state().a_register);
    ASSERT_EQ(7, emulator.get_current_state().b_register);
    ASSERT_EQ(5, emulator.get_current_state().lower_register);
    ASSERT_EQ(0, emulator.get_current_state().upper_register);
    ASSERT_EQ(5, emulator.get_current_state().program_counter);

    // execute b end
    emulator.advance_one_instruction();
    ASSERT_EQ(5, emulator.get_current_state().a_register);
    ASSERT_EQ(7, emulator.get_current_state().b_register);
    ASSERT_EQ(5, emulator.get_current_state().lower_register);
    ASSERT_EQ(0, emulator.get_current_state().upper_register);
    ASSERT_EQ(5, emulator.get_current_state().program_counter);
}

TEST(IntegrationTest, Add_carry) {
    // clang-format off
    const ProgramRom prog_rom =
        assembler::assemble({
            "ldi r0 128", 
            "ldi r1 128", 
            "add r0", 
            "ldi r0 30",
            "ldi r1 7", 
            "addc r0"});
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