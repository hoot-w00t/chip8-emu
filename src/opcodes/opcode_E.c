#include "chip8/defs.h"
#include "logger.h"

int opcode_EX9E(const byte_t x, chip8_system_t *c8)
{
    byte_t Vx = c8->V[x];

    if (c8->keys[Vx]) {
        logger(LOG_DEBUG, "opcode 0xEX9E: Key %X pressed, skipping next instruction", Vx);
        c8->pc += 4;
    } else {
        logger(LOG_DEBUG, "opcode 0xEX9E: Key %X not pressed, not skipping next instruction", Vx);
        c8->pc += 2;
    }

    return 0;
}

int opcode_EXA1(const byte_t x, chip8_system_t *c8)
{
    byte_t Vx = c8->V[x];

    if (!c8->keys[Vx]) {
        logger(LOG_DEBUG, "opcode 0xEXA1: Key %X not pressed, skipping next instruction", Vx);
        c8->pc += 4;
    } else {
        logger(LOG_DEBUG, "opcode 0xEXA1: Key %X pressed, not skipping next instruction", Vx);
        c8->pc += 2;
    }

    return 0;
}

int opcode_E(const uint16_t opcode, chip8_system_t *c8)
{
    byte_t x = (opcode & 0x0F00) >> 8;

    switch (opcode & 0xF0FF) {
        case 0xE09E: return opcode_EX9E(x, c8);
        case 0xE0A1: return opcode_EXA1(x, c8);
        default: return -2; // unknown opcode
    }
}