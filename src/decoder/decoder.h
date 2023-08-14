#ifndef DECODER_H
#define DECODER_H

#include <array>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <cpu/cpu.h>

namespace decoder {

std::array<uint16_t, cpu::DECODER_SIZE> generate_decode_logic();
void generate_decoder_images(std::array<uint16_t, cpu::DECODER_SIZE> &rom);

} // namespace decoder

#endif