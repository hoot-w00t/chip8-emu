#include "chip8/defs.h"
#include "logger.h"

int opcode_B(const uint16_t opcode, chip8_system_t *c8)
{
    uint16_t jmp_address = (opcode & 0x0FFF) + c8->V[0x0];

    logger(LOG_DEBUG, "opcode 0xBNNN: jumping to address 0x%04X (NNN + V0)", jmp_address);
    c8->pc = jmp_address;

    return 0;
}