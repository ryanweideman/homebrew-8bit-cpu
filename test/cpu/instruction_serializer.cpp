#include <gtest/gtest.h>

#include <cpu/instruction_serializer.h>

using std::vector;

TEST(InstructionSerializerTest, Serialize_add) {
    vector<uint8_t> expected = {0x01};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"add", "B"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_addi) {
    vector<uint8_t> expected = {0x05, 0x05};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"addi", "B", "5"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_addc) {
    vector<uint8_t> expected = {0x0b};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"addc", "U"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_sub) {
    vector<uint8_t> expected = {0x0f};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"sub", "U"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_subi) {
    vector<uint8_t> expected = {0x13, 0xff};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"subi", "U", "255"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_subc) {
    vector<uint8_t> expected = {0x16};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"subc", "L"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_not) {
    vector<uint8_t> expected = {0x19};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"not", "B"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_and) {
    vector<uint8_t> expected = {0x1d};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"and", "B"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_andi) {
    vector<uint8_t> expected = {0x21, 0xff};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"andi", "B", "255"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_or) {
    vector<uint8_t> expected = {0x26};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"or", "L"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_ori) {
    vector<uint8_t> expected = {0x2a, 0xff};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"ori", "L", "255"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_xor) {
    vector<uint8_t> expected = {0x2e};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"xor", "L"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_xori) {
    vector<uint8_t> expected = {0x32, 0xff};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"xori", "L", "255"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_shl) {
    vector<uint8_t> expected = {0x37};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"shl", "U"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_shlc) {
    vector<uint8_t> expected = {0x3b};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"shlc", "U"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_ldi) {
    vector<uint8_t> expected = {0x3f, 0x7f};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"ldi", "U", "127"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_ld) {
    vector<uint8_t> expected = {0x41};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"ld", "B"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_lda) {
    vector<uint8_t> expected = {0x45, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"lda", "B", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_st) {
    vector<uint8_t> expected = {0x48};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"st", "A"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_sta) {
    vector<uint8_t> expected = {0x4c, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"sta", "A", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_stia) {
    vector<uint8_t> expected = {0x54, 0x7f, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"stia", "127", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_cmp) {
    vector<uint8_t> expected = {0x58};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"cmp"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_b) {
    vector<uint8_t> expected = {0x5c, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"b", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_bc) {
    vector<uint8_t> expected = {0x60, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"bc", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_blt) {
    vector<uint8_t> expected = {0x64, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"blt", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_blte) {
    vector<uint8_t> expected = {0x68, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"blte", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_be) {
    vector<uint8_t> expected = {0x6c, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"be", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_bne) {
    vector<uint8_t> expected = {0x70, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"bne", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_bgt) {
    vector<uint8_t> expected = {0x74, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"bgt", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_bgte) {
    vector<uint8_t> expected = {0x78, 0xe8, 0x03};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"bgte", "1000"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_mov) {
    vector<uint8_t> expected_mova = {0x7e};
    vector<uint8_t> actual_mova =
        instruction_serializer::serialize_instruction({"mov", "A", "L"});
    EXPECT_EQ(expected_mova, actual_mova);

    vector<uint8_t> expected_movb = {0x82};
    vector<uint8_t> actual_movb =
        instruction_serializer::serialize_instruction({"mov", "B", "L"});
    EXPECT_EQ(expected_movb[0], actual_movb[0]);
}

TEST(InstructionSerializerTest, Serialize_nop) {
    vector<uint8_t> expected = {0x84};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"nop"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_setc) {
    vector<uint8_t> expected = {0x88};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"setc"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_clc) {
    vector<uint8_t> expected = {0x8c};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"clc"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Serialize_out) {
    vector<uint8_t> expected = {0x90, 0x01, 0xff};
    vector<uint8_t> actual =
        instruction_serializer::serialize_instruction({"out", "1", "255"});
    EXPECT_EQ(expected, actual);
}

TEST(InstructionSerializerTest, Deserialize_add) {
    std::vector<std::string> expected = {"add", "B"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x01}));
}

TEST(InstructionSerializerTest, Deserialize_addi) {
    std::vector<std::string> expected = {"addi", "B", "x05"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x05, 0x05}));
}

TEST(InstructionSerializerTest, Deserialize_addc) {
    std::vector<std::string> expected = {"addc", "U"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x0b}));
}

TEST(InstructionSerializerTest, Deserialize_sub) {
    std::vector<std::string> expected = {"sub", "U"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x0f}));
}

TEST(InstructionSerializerTest, Deserialize_subi) {
    std::vector<std::string> expected = {"subi", "U", "xff"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x13, 0xff}));
}

TEST(InstructionSerializerTest, Deserialize_subc) {
    std::vector<std::string> expected = {"subc", "L"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x16}));
}

TEST(InstructionSerializerTest, Deserialize_not) {
    std::vector<std::string> expected = {"not", "B"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x19}));
}

TEST(InstructionSerializerTest, Deserialize_and) {
    std::vector<std::string> expected = {"and", "B"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x1d}));
}

TEST(InstructionSerializerTest, Deserialize_andi) {
    std::vector<std::string> expected = {"andi", "B", "xff"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x21, 0xff}));
}

TEST(InstructionSerializerTest, Deserialize_or) {
    std::vector<std::string> expected = {"or", "L"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x26}));
}

TEST(InstructionSerializerTest, Deserialize_ori) {
    std::vector<std::string> expected = {"ori", "L", "xff"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x2a, 0xff}));
}

TEST(InstructionSerializerTest, Deserialize_xor) {
    std::vector<std::string> expected = {"xor", "L"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x2e}));
}

TEST(InstructionSerializerTest, Deserialize_xori) {
    std::vector<std::string> expected = {"xori", "L", "xff"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x32, 0xff}));
}

TEST(InstructionSerializerTest, Deserialize_shl) {
    std::vector<std::string> expected = {"shl", "U"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x37}));
}

TEST(InstructionSerializerTest, Deserialize_shlc) {
    std::vector<std::string> expected = {"shlc", "U"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x3b}));
}

TEST(InstructionSerializerTest, Deserialize_ldi) {
    std::vector<std::string> expected = {"ldi", "U", "x7f"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x3f, 0x7f}));
}

TEST(InstructionSerializerTest, Deserialize_ld) {
    std::vector<std::string> expected = {"ld", "B"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x41}));
}

TEST(InstructionSerializerTest, Deserialize_lda) {
    std::vector<std::string> expected = {"lda", "B", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x45, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_st) {
    std::vector<std::string> expected = {"st", "A"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x48}));
}

TEST(InstructionSerializerTest, Deserialize_sta) {
    std::vector<std::string> expected = {"sta", "A", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x4c, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_stia) {
    std::vector<std::string> expected = {"stia", "x7f", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x54, 0x7f, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_cmp) {
    std::vector<std::string> expected = {"cmp"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x58}));
}

TEST(InstructionSerializerTest, Deserialize_b) {
    std::vector<std::string> expected = {"b", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x5c, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_bc) {
    std::vector<std::string> expected = {"bc", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x60, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_blt) {
    std::vector<std::string> expected = {"blt", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x64, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_blte) {
    std::vector<std::string> expected = {"blte", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x68, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_be) {
    std::vector<std::string> expected = {"be", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x6c, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_bne) {
    std::vector<std::string> expected = {"bne", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x70, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_bgt) {
    std::vector<std::string> expected = {"bgt", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x74, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_bgte) {
    std::vector<std::string> expected = {"bgte", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x78, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_mov) {
    std::vector<std::string> expected1 = {"mov", "A", "L"};
    EXPECT_EQ(expected1,
              instruction_serializer::deserialize_instruction({0x7e}));
    std::vector<std::string> expected2 = {"mov", "B", "L"};
    EXPECT_EQ(expected2,
              instruction_serializer::deserialize_instruction({0x82}));
}

TEST(InstructionSerializerTest, Deserialize_nop) {
    std::vector<std::string> expected1 = {"nop"};
    EXPECT_EQ(expected1,
              instruction_serializer::deserialize_instruction({0x84}));
}

TEST(InstructionSerializerTest, Deserialize_setc) {
    std::vector<std::string> expected1 = {"setc"};
    EXPECT_EQ(expected1,
              instruction_serializer::deserialize_instruction({0x88}));
}

TEST(InstructionSerializerTest, Deserialize_clc) {
    std::vector<std::string> expected1 = {"clc"};
    EXPECT_EQ(expected1,
              instruction_serializer::deserialize_instruction({0x8c}));
}

TEST(InstructionSerializerTest, Deserialize_out) {
    std::vector<std::string> expected1 = {"out", "1", "xff"};
    EXPECT_EQ(expected1, instruction_serializer::deserialize_instruction(
                             {0x90, 0x01, 0xff}));
}