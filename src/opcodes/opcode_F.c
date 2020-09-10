#include "chip8/defs.h"
#include "logger.h"

int opcode_FX07(const byte_t x, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0xFX07: set V%x to %u", x, c8->delay_timer);
    c8->V[x] = c8->delay_timer;
    c8->pc += 2;

    return 0;
}

int opcode_FX0A(const byte_t x, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0xFX0A: set V%x to key press", x);
    c8->keypress_reg = x;
    c8->keypress = 0xFF;
    c8->pc += 2;

    return 0;
}

int opcode_FX15(const byte_t x, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0xFX15: set delay timer to 0x%02X (V%x)", c8->V[x], x);
    c8->delay_timer = c8->V[x];
    c8->pc += 2;

    return 0;
}

int opcode_FX18(const byte_t x, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0xFX18: set sound timer to 0x%02X (V%x)", c8->V[x], x);
    c8->sound_timer = c8->V[x];
    c8->pc += 2;

    return 0;
}

int opcode_FX1E(const byte_t x, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0xFX1E: I += 0x%02X (V%x)", c8->V[x], x);
    c8->I += c8->V[x];
    c8->pc += 2;

    return 0;
}

int opcode_FX29(const byte_t x, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0xFX29: loading sprite address for 0x%X in I", c8->V[x]);
    c8->I = c8->V[x] * 0x5;
    c8->pc += 2;

    return 0;
}

int opcode_FX33(const byte_t x, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0xFX33: BCD");
    c8->memory[c8->I] = c8->V[x] / 100;
    c8->memory[c8->I + 1] = (c8->V[x] / 10) % 10;
    c8->memory[c8->I + 2] = (c8->V[x] % 100) % 10;

    c8->pc += 2;

    return 0;
}

int opcode_FX55(const byte_t x, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0xFX55: dump registers from V0 to V%x to 0x%04X", x, c8->I);

    for (byte_t i = 0; i <= x; ++i)
        c8->memory[c8->I + i] = c8->V[i];
    c8->pc += 2;

    return 0;
}

int opcode_FX65(const byte_t x, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0xFX65: load registers from V0 to V%x from 0x%04X", x, c8->I);

    for (byte_t i = 0; i <= x; ++i)
        c8->V[i] = c8->memory[c8->I + i];
    c8->pc += 2;

    return 0;
}

int opcode_F(const uint16_t opcode, chip8_system_t *c8)
{
    byte_t x = (opcode & 0x0F00) >> 8;

    switch (opcode & 0x00FF) {
        case 0x0007: return opcode_FX07(x, c8);
        case 0x000A: return opcode_FX0A(x, c8);
        case 0x0015: return opcode_FX15(x, c8);
        case 0x0018: return opcode_FX18(x, c8);
        case 0x001E: return opcode_FX1E(x, c8);
        case 0x0029: return opcode_FX29(x, c8);
        case 0x0033: return opcode_FX33(x, c8);
        case 0x0055: return opcode_FX55(x, c8);
        case 0x0065: return opcode_FX65(x, c8);
        default: return -2; // unknown opcode
    }
}