#include "decoder.h"

int main() {
    std::array<uint16_t, cpu::DECODER_SIZE> decoder_rom =
        decoder::generate_decode_logic();
    decoder::generate_decoder_images(decoder_rom);
}