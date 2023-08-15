#include <gtest/gtest.h>

#include <cpu/instruction_serializer.h>

TEST(InstructionSerializerTest, Serialize_add) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"add", "r1"}).c_str(),
        "01 ");
}

TEST(InstructionSerializerTest, Serialize_addi) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"addi", "r1", "5"})
            .c_str(),
        "05 05 ");
}

TEST(InstructionSerializerTest, Serialize_addc) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"addc", "r3"}).c_str(),
        "0b ");
}

TEST(InstructionSerializerTest, Serialize_sub) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"sub", "r3"}).c_str(),
        "0f ");
}

TEST(InstructionSerializerTest, Serialize_subi) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"subi", "r3", "255"})
            .c_str(),
        "13 ff ");
}

TEST(InstructionSerializerTest, Serialize_subc) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"subc", "r2"}).c_str(),
        "16 ");
}

TEST(InstructionSerializerTest, Serialize_not) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"not", "r1"}).c_str(),
        "19 ");
}

TEST(InstructionSerializerTest, Serialize_and) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"and", "r1"}).c_str(),
        "1d ");
}

TEST(InstructionSerializerTest, Serialize_andi) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"andi", "r1", "255"})
            .c_str(),
        "21 ff ");
}

TEST(InstructionSerializerTest, Serialize_or) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"or", "r2"}).c_str(),
        "26 ");
}

TEST(InstructionSerializerTest, Serialize_ori) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"ori", "r2", "255"})
            .c_str(),
        "2a ff ");
}

TEST(InstructionSerializerTest, Serialize_xor) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"xor", "r2"}).c_str(),
        "2e ");
}

TEST(InstructionSerializerTest, Serialize_xori) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"xori", "r2", "255"})
            .c_str(),
        "32 ff ");
}

TEST(InstructionSerializerTest, Serialize_shl) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"shl", "r3"}).c_str(),
        "37 ");
}

TEST(InstructionSerializerTest, Serialize_shlc) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"shlc", "r3"}).c_str(),
        "3b ");
}

TEST(InstructionSerializerTest, Serialize_ldi) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"ldi", "r3", "127"})
            .c_str(),
        "3f 7f ");
}

TEST(InstructionSerializerTest, Serialize_ld) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"ld", "r1"}).c_str(),
        "41 ");
}

TEST(InstructionSerializerTest, Serialize_lda) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"lda", "r1", "1000"})
            .c_str(),
        "45 e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_st) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"st", "r0"}).c_str(),
        "48 ");
}

TEST(InstructionSerializerTest, Serialize_sta) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"sta", "r0", "1000"})
            .c_str(),
        "4c e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_stia) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"stia", "127", "1000"})
            .c_str(),
        "54 7f e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_cmp) {
    EXPECT_STREQ(instruction_serializer::serialize_instruction({"cmp"}).c_str(),
                 "58 ");
}

TEST(InstructionSerializerTest, Serialize_b) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"b", "1000"}).c_str(),
        "5c e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_bc) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"bc", "1000"}).c_str(),
        "60 e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_blt) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"blt", "1000"}).c_str(),
        "64 e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_blte) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"blte", "1000"}).c_str(),
        "68 e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_be) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"be", "1000"}).c_str(),
        "6c e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_bne) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"bne", "1000"}).c_str(),
        "70 e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_bgt) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"bgt", "1000"}).c_str(),
        "74 e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_bgte) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"bgte", "1000"}).c_str(),
        "78 e8 03 ");
}

TEST(InstructionSerializerTest, Serialize_mov) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"mov", "r0", "r2"})
            .c_str(),
        "7e ");
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"mov", "r1", "r2"})
            .c_str(),
        "82 ");
}

TEST(InstructionSerializerTest, Serialize_nop) {
    EXPECT_STREQ(instruction_serializer::serialize_instruction({"nop"}).c_str(),
                 "84 ");
}

TEST(InstructionSerializerTest, Serialize_setc) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"setc"}).c_str(), "88 ");
}

TEST(InstructionSerializerTest, Serialize_clc) {
    EXPECT_STREQ(instruction_serializer::serialize_instruction({"clc"}).c_str(),
                 "8c ");
}

TEST(InstructionSerializerTest, Serialize_out) {
    EXPECT_STREQ(
        instruction_serializer::serialize_instruction({"out", "1", "255"})
            .c_str(),
        "90 01 ff ");
}

TEST(InstructionSerializerTest, Deserialize_add) {
    std::vector<std::string> expected = {"add", "r1"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x01}));
}

TEST(InstructionSerializerTest, Deserialize_addi) {
    std::vector<std::string> expected = {"addi", "r1", "x05"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x05, 0x05}));
}

TEST(InstructionSerializerTest, Deserialize_addc) {
    std::vector<std::string> expected = {"addc", "r3"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x0b}));
}

TEST(InstructionSerializerTest, Deserialize_sub) {
    std::vector<std::string> expected = {"sub", "r3"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x0f}));
}

TEST(InstructionSerializerTest, Deserialize_subi) {
    std::vector<std::string> expected = {"subi", "r3", "xff"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x13, 0xff}));
}

TEST(InstructionSerializerTest, Deserialize_subc) {
    std::vector<std::string> expected = {"subc", "r2"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x16}));
}

TEST(InstructionSerializerTest, Deserialize_not) {
    std::vector<std::string> expected = {"not", "r1"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x19}));
}

TEST(InstructionSerializerTest, Deserialize_and) {
    std::vector<std::string> expected = {"and", "r1"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x1d}));
}

TEST(InstructionSerializerTest, Deserialize_andi) {
    std::vector<std::string> expected = {"andi", "r1", "xff"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x21, 0xff}));
}

TEST(InstructionSerializerTest, Deserialize_or) {
    std::vector<std::string> expected = {"or", "r2"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x26}));
}

TEST(InstructionSerializerTest, Deserialize_ori) {
    std::vector<std::string> expected = {"ori", "r2", "xff"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x2a, 0xff}));
}

TEST(InstructionSerializerTest, Deserialize_xor) {
    std::vector<std::string> expected = {"xor", "r2"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x2e}));
}

TEST(InstructionSerializerTest, Deserialize_xori) {
    std::vector<std::string> expected = {"xori", "r2", "xff"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x32, 0xff}));
}

TEST(InstructionSerializerTest, Deserialize_shl) {
    std::vector<std::string> expected = {"shl", "r3"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x37}));
}

TEST(InstructionSerializerTest, Deserialize_shlc) {
    std::vector<std::string> expected = {"shlc", "r3"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x3b}));
}

TEST(InstructionSerializerTest, Deserialize_ldi) {
    std::vector<std::string> expected = {"ldi", "r3", "x7f"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x3f, 0x7f}));
}

TEST(InstructionSerializerTest, Deserialize_ld) {
    std::vector<std::string> expected = {"ld", "r1"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x41}));
}

TEST(InstructionSerializerTest, Deserialize_lda) {
    std::vector<std::string> expected = {"lda", "r1", "x03e8"};
    EXPECT_EQ(expected, instruction_serializer::deserialize_instruction(
                            {0x45, 0xe8, 0x03}));
}

TEST(InstructionSerializerTest, Deserialize_st) {
    std::vector<std::string> expected = {"st", "r0"};
    EXPECT_EQ(expected,
              instruction_serializer::deserialize_instruction({0x48}));
}

TEST(InstructionSerializerTest, Deserialize_sta) {
    std::vector<std::string> expected = {"sta", "r0", "x03e8"};
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
    std::vector<std::string> expected1 = {"mov", "r0", "r2"};
    EXPECT_EQ(expected1,
              instruction_serializer::deserialize_instruction({0x7e}));
    std::vector<std::string> expected2 = {"mov", "r1", "r2"};
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
