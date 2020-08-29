#include "chip8/defs.h"
#include "logger.h"
#include <stdlib.h>

int opcode_C(const uint16_t opcode, chip8_system_t *c8)
{
    byte_t x = (opcode & 0x0F00) >> 8;
    byte_t NN = opcode & 0x00FF;
    byte_t rnd = rand() & NN;

    logger(LOG_DEBUG, "opcode 0xCXNN: set V%x to random 0x%02X", x, rnd);
    c8->V[x] = rnd;
    c8->pc += 2;

    return 0;
}