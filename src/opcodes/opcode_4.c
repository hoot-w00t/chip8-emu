#include "chip8/defs.h"
#include "logger.h"

int opcode_4(const uint16_t opcode, chip8_system_t *c8)
{
    byte_t Vx = c8->V[(opcode & 0x0F00) >> 8];
    byte_t NN = opcode & 0x00FF;

    if (Vx != NN) {
        logger(LOG_DEBUG, "opcode 0x4XNN: VX (0x%02X) != NN (0x%02X), skipping next instruction", Vx, NN);
        c8->pc += 4;
    } else {
        logger(LOG_DEBUG, "opcode 0x4XNN: VX (0x%02X) == NN (0x%02X), not skipping next instruction", Vx, NN);
        c8->pc += 2;
    }

    return 0;
}