#include "chip8/defs.h"
#include "logger.h"
#include <string.h>

int opcode_0NNN(const uint16_t opcode, chip8_system_t *c8)
{
    uint16_t jmp_address = opcode & 0x0FFF;

    logger(LOG_WARN, "opcode 0x0NNN: Ignored: call machine code routine at 0x%04X", jmp_address);
    c8->pc += 2;

    return 0;
}

int opcode_00E0(chip8_system_t *c8)
{
    logger(LOG_DEBUG, "opcode 0x00E0: clearing the screen");
    memset(c8->video_memory, 0, sizeof(c8->video_memory));
    c8->pc += 2;

    return 0;
}

int opcode_00EE(chip8_system_t *c8)
{
    if (c8->stack_ptr == 0) {
        logger(LOG_CRIT, "opcode 0x00EE: stack is empty");
        c8->pc += 2;
        return 0;
    }

    c8->stack_ptr -= 1;
    uint16_t jmp_address = c8->stack[c8->stack_ptr] + 2;

    logger(LOG_DEBUG, "opcode 0x00EE: returning to address 0x%04X", jmp_address);
    c8->pc = jmp_address;

    return 0;
}

int opcode_0(const uint16_t opcode, chip8_system_t *c8)
{
    switch (opcode & 0x0FFF) {
        case 0x00E0: return opcode_00E0(c8);
        case 0x00EE: return opcode_00EE(c8);
        default: return opcode_0NNN(opcode, c8);
    }
}