#include "chip8/defs.h"
#include "logger.h"

int opcode_1(const uint16_t opcode, chip8_system_t *c8)
{
    uint16_t jmp_address = opcode & 0x0FFF;

    logger(LOG_DEBUG, "opcode 0x1NNN: jump to address 0x%04X", jmp_address);
    c8->pc = jmp_address;

    return 0;
}