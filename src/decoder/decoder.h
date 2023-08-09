#ifndef DECODER_H
#define DECODER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector>

std::array<uint16_t, cpu::DECODER_SIZE> generate_decode_logic();
void generate_decode_logic_and_files();

#endif