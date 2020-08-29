#include "chip8/defs.h"
#include "logger.h"

int opcode_8XY0(byte_t x, byte_t y, chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0x8XY0: set V%x (0x%02X) to V%x (0x%02X)",
                      x,
                      c8->V[x],
                      y,
                      c8->V[y]);

    c8->V[x] = c8->V[y];
    c8->pc += 2;

    return 0;
}

int opcode_8XY1(byte_t x, byte_t y, chip8_system_t *c8)
{
    byte_t result = c8->V[x] | c8->V[y];

    logger(LOG_DEBUG, "opcode 0x8XY1: set V%x to V%x | V%x (0x%02X)",
                      x,
                      x,
                      y,
                      result);

    c8->V[x] = result;
    c8->pc += 2;

    return 0;
}

int opcode_8XY2(byte_t x, byte_t y, chip8_system_t *c8)
{
    byte_t result = c8->V[x] & c8->V[y];

    logger(LOG_DEBUG, "opcode 0x8XY2: set V%x to V%x & V%x (0x%02X)",
                      x,
                      x,
                      y,
                      result);

    c8->V[x] = result;
    c8->pc += 2;

    return 0;
}

int opcode_8XY3(byte_t x, byte_t y, chip8_system_t *c8)
{
    byte_t result = c8->V[x] ^ c8->V[y];

    logger(LOG_DEBUG, "opcode 0x8XY3: set V%x to V%x ^ V%x (0x%02X)",
                      x,
                      x,
                      y,
                      result);

    c8->V[x] = result;
    c8->pc += 2;

    return 0;
}

int opcode_8XY4(byte_t x, byte_t y, chip8_system_t *c8)
{
    byte_t result = c8->V[x] + c8->V[y];
    byte_t carry = (c8->V[x] > (0xFF - c8->V[y])) ? 1 : 0;

    logger(LOG_DEBUG, "opcode 0x8XY4: 0x%02X (V%x) += 0x%02X (V%x), set VF to 0x%02X",
                      c8->V[x],
                      x,
                      c8->V[y],
                      y,
                      carry);

    c8->V[x] = result;
    c8->V[0xF] = carry;
    c8->pc += 2;

    return 0;
}

int opcode_8XY5(byte_t x, byte_t y, chip8_system_t *c8)
{
    byte_t result = c8->V[x] - c8->V[y];
    byte_t borrow = c8->V[y] > c8->V[x] ? 0 : 1;

    logger(LOG_DEBUG, "opcode 0x8XY5: 0x%02X (V%x) -= 0x%02X (V%x), set VF to 0x%02X",
                      c8->V[x],
                      x,
                      c8->V[y],
                      y,
                      borrow);

    c8->V[x] = result;
    c8->V[0xF] = borrow;
    c8->pc += 2;

    return 0;
}

int opcode_8XY6(byte_t x, __attribute__((unused)) byte_t y, chip8_system_t *c8)
{
    byte_t result = c8->V[x] >> 1;
    byte_t lsb = c8->V[x] & 0x1;

    logger(LOG_DEBUG, "opcode 0x8XY6: 0x%02X (V%x) >>= 1, set VF to 0x%02X",
                      c8->V[x],
                      x,
                      lsb);

    c8->V[x] = result;
    c8->V[0xF] = lsb;
    c8->pc += 2;

    return 0;
}

int opcode_8XY7(byte_t x, byte_t y, chip8_system_t *c8)
{
    byte_t result = c8->V[y] - c8->V[x];
    byte_t borrow = c8->V[x] > c8->V[y] ? 0 : 1;

    logger(LOG_DEBUG, "opcode 0x8XY7: set V%x to 0x%02X (V%x) - 0x%02X (V%x), set VF to 0x%02X",
                      x,
                      c8->V[y],
                      y,
                      c8->V[x],
                      x,
                      borrow);

    c8->V[x] = result;
    c8->V[0xF] = borrow;
    c8->pc += 2;

    return 0;
}

int opcode_8XYE(byte_t x, __attribute__((unused)) byte_t y, chip8_system_t *c8)
{
    byte_t result = c8->V[x] << 1;
    byte_t msb = (c8->V[x] >> 7) & 0x1;

    logger(LOG_DEBUG, "opcode 0x8XYE: 0x%02X (V%x) <<= 1, set VF to 0x%02X",
                      c8->V[x],
                      x,
                      msb);

    c8->V[x] = result;
    c8->V[0xF] = msb;
    c8->pc += 2;

    return 0;
}

int opcode_8(const uint16_t opcode, chip8_system_t *c8)
{
    byte_t x = (opcode & 0x0F00) >> 8;
    byte_t y = (opcode & 0x00F0) >> 4;

    switch (opcode & 0x000F) {
        case 0x0000: return opcode_8XY0(x, y, c8);
        case 0x0001: return opcode_8XY1(x, y, c8);
        case 0x0002: return opcode_8XY2(x, y, c8);
        case 0x0003: return opcode_8XY3(x, y, c8);
        case 0x0004: return opcode_8XY4(x, y, c8);
        case 0x0005: return opcode_8XY5(x, y, c8);
        case 0x0006: return opcode_8XY6(x, y, c8);
        case 0x0007: return opcode_8XY7(x, y, c8);
        case 0x000E: return opcode_8XYE(x, y, c8);
        default: return -2; // unknown opcode
    }
}