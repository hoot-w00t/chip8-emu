#include "chip8/defs.h"
#include "logger.h"

int opcode_6(const uint16_t opcode, chip8_system_t *c8)
{
    byte_t x = (opcode & 0x0F00) >> 8;
    byte_t NN = opcode & 0x00FF;

    logger(LOG_DEBUG, "opcode 0x6XNN: set V%x to 0x%02X", x, NN);
    c8->V[x] = NN;
    c8->pc += 2;

    return 0;
}