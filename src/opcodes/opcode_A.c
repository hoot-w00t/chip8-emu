#include "chip8/defs.h"
#include "logger.h"

int opcode_A(const uint16_t opcode, chip8_system_t *c8)
{
    uint16_t address = opcode & 0x0FFF;

    logger(LOG_DEBUG, "opcode 0xANNN: set I to address 0x%04X", address);
    c8->I = address;
    c8->pc += 2;

    return 0;
}