#include "chip8/defs.h"
#include "logger.h"

int opcode_5(const uint16_t opcode, chip8_system_t *c8)
{
    if (opcode & 0x000F)
        return -2; // Unknown opcode

    byte_t Vx = c8->V[(opcode & 0x0F00) >> 8];
    byte_t Vy = c8->V[(opcode & 0x00F0) >> 4];

    if (Vx == Vy) {
        logger(LOG_DEBUG, "opcode 0x5XY0: VX (0x%02X) == Vy (0x%02X), skipping next instruction", Vx, Vy);
        c8->pc += 4;
    } else {
        logger(LOG_DEBUG, "opcode 0x5XY0: VX (0x%02X) != Vy (0x%02X), not skipping next instruction", Vx, Vy);
        c8->pc += 2;
    }

    return 0;
}