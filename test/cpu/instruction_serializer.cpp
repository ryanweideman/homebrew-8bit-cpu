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
