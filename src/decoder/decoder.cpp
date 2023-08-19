#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tuple>
#include <vector>

#include <cpu/cpu.h>

namespace decoder {

void write_data(std::array<uint16_t, cpu::DECODER_SIZE> &data, uint8_t c,
                uint8_t z, uint8_t n, uint8_t opcode, uint8_t reg,
                std::vector<uint16_t> micro) {
    uint16_t addr = (c << 4) | (z << 5) | (n << 6) | (reg << 7) | (opcode << 9);

    // Add fetch cycle
    micro.insert(micro.begin(), cpu::U_FETCH);

    // Reset microcode on last microcode instruction
    micro.back() &= cpu::U_UCODE_RESET;

    std::copy_n(micro.begin(), micro.size(), data.begin() + addr);
}

// returns the microcode to enable a given register
uint16_t reg_en(uint8_t reg) {
    uint16_t uc;
    switch (reg) {
    case cpu::A_REG:
        uc = cpu::U_A_EN;
        break;
    case cpu::B_REG:
        uc = cpu::U_B_EN;
        break;
    case cpu::L_REG:
        uc = cpu::U_L_EN;
        break;
    case cpu::U_REG:
        uc = cpu::U_U_EN;
        break;
    }
    return uc;
}

std::array<uint16_t, cpu::DECODER_SIZE> generate_decode_logic() {
    std::array<uint16_t, cpu::DECODER_SIZE> rom;
    rom.fill(0);

    std::vector<cpu::Opcode> opcodes;
    for (const auto &instruction : cpu::get_instructions()) {
        opcodes.push_back(instruction.opcode_);
    }

    std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> flags{
        std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 1),
        std::make_tuple(0, 1, 0), std::make_tuple(0, 1, 1),
        std::make_tuple(1, 0, 0), std::make_tuple(1, 0, 1),
        std::make_tuple(1, 1, 0), std::make_tuple(1, 1, 1),
    };

    for (auto const &[n, z, c] : flags) {
        for (uint8_t reg = 0; reg < 4; reg++) {
            for (uint8_t i = 0; i < 64; i++) {
                std::vector<uint16_t> microcode;
                microcode.push_back(cpu::U_NOP);
                write_data(rom, c, z, n, i, reg, microcode);
            }
            for (const cpu::Opcode opcode : opcodes) {
                std::vector<uint16_t> microcode;

                switch (opcode) {
                case cpu::Opcode::ADD:
                    microcode.push_back(cpu::U_ADD | reg_en(reg));
                    break;

                case cpu::Opcode::ADDI:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::B_REG));
                    microcode.push_back(cpu::U_ADD | reg_en(reg));
                    break;

                case cpu::Opcode::ADDC:
                    if (c == 0) {
                        microcode.push_back(cpu::U_ADDC | reg_en(reg));
                    } else {
                        microcode.push_back(cpu::U_ADD | reg_en(reg));
                    }
                    break;

                case cpu::Opcode::SUB:
                    microcode.push_back(cpu::U_SUB | reg_en(reg));
                    break;

                case cpu::Opcode::SUBI:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::B_REG));
                    microcode.push_back(cpu::U_SUB | reg_en(reg));
                    break;

                case cpu::Opcode::SUBC:
                    if (c == 0) {
                        microcode.push_back(cpu::U_SUBC | reg_en(reg));
                    } else {
                        microcode.push_back(cpu::U_SUB | reg_en(reg));
                    }
                    break;

                case cpu::Opcode::NOT:
                    microcode.push_back(cpu::U_NOT_A | reg_en(reg));
                    break;

                case cpu::Opcode::AND:
                    microcode.push_back(cpu::U_AND | reg_en(reg));
                    break;

                case cpu::Opcode::ANDI:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::B_REG));
                    microcode.push_back(cpu::U_AND | reg_en(reg));
                    break;

                case cpu::Opcode::OR:
                    microcode.push_back(cpu::U_OR | reg_en(reg));
                    break;

                case cpu::Opcode::ORI:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::B_REG));
                    microcode.push_back(cpu::U_OR | reg_en(reg));
                    break;

                case cpu::Opcode::XOR:
                    microcode.push_back(cpu::U_XOR | reg_en(reg));
                    break;

                case cpu::Opcode::XORI:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::B_REG));
                    microcode.push_back(cpu::U_XOR | reg_en(reg));
                    break;

                case cpu::Opcode::SHL:
                    microcode.push_back(cpu::U_SHL | reg_en(reg));
                    break;

                case cpu::Opcode::SHLC:
                    microcode.push_back(cpu::U_SHLC | reg_en(reg));
                    break;

                case cpu::Opcode::LD:
                    microcode.push_back(cpu::U_LD | reg_en(reg));
                    break;

                case cpu::Opcode::LDI:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(reg));
                    break;

                case cpu::Opcode::LDA:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    microcode.push_back(cpu::U_LD | reg_en(reg));
                    break;

                case cpu::Opcode::ST:
                    microcode.push_back(cpu::U_ST_A);
                    break;

                case cpu::Opcode::STI:
                    microcode.push_back(cpu::U_STI);
                    break;

                case cpu::Opcode::STA:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    if (reg == 1) {
                        microcode.push_back(cpu::U_ST_B);
                    } else {
                        microcode.push_back(cpu::U_ST_A);
                    }
                    break;

                case cpu::Opcode::STIA:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    microcode.push_back(cpu::U_STI);
                    break;

                case cpu::Opcode::CMP:
                    microcode.push_back(cpu::U_CMP);
                    break;

                case cpu::Opcode::B:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    microcode.push_back(cpu::U_B);
                    break;

                case cpu::Opcode::BC:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    if (c == 0) {
                        microcode.push_back(cpu::U_B);
                    }
                    break;

                case cpu::Opcode::BLT:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    if (z == 0 && c == 1) {
                        microcode.push_back(cpu::U_B);
                    }
                    break;

                case cpu::Opcode::BLTE:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    if ((z == 1 && c == 1) || z == 0) {
                        microcode.push_back(cpu::U_B);
                    }
                    break;

                case cpu::Opcode::BGT:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    if (z == 0 && c == 0) {
                        microcode.push_back(cpu::U_B);
                    }
                    break;

                case cpu::Opcode::BGTE:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    if ((z == 1 && c == 0) || z == 0) {
                        microcode.push_back(cpu::U_B);
                    }
                    break;

                case cpu::Opcode::BE:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    if (z == 1) {
                        microcode.push_back(cpu::U_B);
                    }
                    break;

                case cpu::Opcode::BNE:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::U_REG));
                    if (z == 0) {
                        microcode.push_back(cpu::U_B);
                    }
                    break;

                case cpu::Opcode::MOVA:
                    microcode.push_back(cpu::U_MOV_A | reg_en(reg));
                    break;

                case cpu::Opcode::MOVB:
                    microcode.push_back(cpu::U_MOV_B | reg_en(reg));
                    break;

                case cpu::Opcode::NOP:
                    microcode.push_back(cpu::U_NOP);
                    break;

                case cpu::Opcode::SETC:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::A_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::B_REG));
                    microcode.push_back(cpu::U_ADD);
                    break;

                case cpu::Opcode::CLC:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::A_REG));
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::B_REG));
                    microcode.push_back(cpu::U_ADD);
                    break;

                case cpu::Opcode::OUT:
                    microcode.push_back(cpu::U_IMM_TO_REG | reg_en(cpu::L_REG));
                    microcode.push_back(cpu::U_OUTI);
                    break;
                }

                uint8_t opcode_value = cpu::get_value_of_opcode(opcode);
                write_data(rom, c, z, n, opcode_value, reg, microcode);
            }
        }
    }

    return rom;
}

std::string byte_to_hex_string(const uint8_t val) {
    std::stringstream stream;
    stream << std::setfill('0') << std::setw(2) << std::right << std::hex
           << static_cast<int>(val);
    return stream.str();
}

void generate_decoder_images(
    std::array<uint16_t, cpu::DECODER_SIZE> &decoder_rom) {
    std::ofstream lower_rom_image;
    lower_rom_image.open("decoder_rom_lower");
    for (int i = 0; i < cpu::DECODER_SIZE; i++) {
        uint8_t d = (uint8_t)(decoder_rom[i] & 0xFF);
        lower_rom_image << byte_to_hex_string(d) << " ";
    }
    lower_rom_image.close();

    std::ofstream upper_rom_image;
    upper_rom_image.open("decoder_rom_upper");
    for (int i = 0; i < cpu::DECODER_SIZE; i++) {
        uint8_t d = (uint8_t)((decoder_rom[i] >> 8) & 0xFF);
        upper_rom_image << byte_to_hex_string(d) << " ";
    }
    upper_rom_image.close();
}

} // namespace decoder