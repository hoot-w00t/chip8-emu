#include "chip8/defs.h"
#include "logger.h"

int opcode_D(const uint16_t opcode, chip8_system_t *c8)
{
    byte_t x = c8->V[(opcode & 0x0F00) >> 8];
    byte_t y = c8->V[(opcode & 0x00F0) >> 4];
    byte_t height = opcode & 0x000F;
    byte_t pixel;

    c8->V[0xF] = 0;
    for (byte_t yl = 0; yl < height; ++yl) { // iterate through each line up to height (N)
        pixel = c8->memory[c8->I + yl];

        for (byte_t xl = 0; xl < 8; ++xl) {
            if (pixel & (0x80 >> xl)) {
                // bit for x value is true, draw it
                if (c8->video_memory[x + xl + ((y + yl) * 64)]) // set VF to 1 because this pixel is inverted
                    c8->V[0xF] = 1;

                c8->video_memory[x + xl + ((y + yl) * 64)] ^= 1;
            }
        }
    }

    logger(
        LOG_DEBUG,
        "opcode 0xDXYN: drawing from memory 0x%04X, at x=%u, y=%u, height=%u",
        c8->I,
        x,
        y,
        height);

    c8->screen_refreshed = 1;
    c8->pc += 2;

    return 0;
}