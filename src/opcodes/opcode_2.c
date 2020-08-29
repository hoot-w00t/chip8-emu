#include "chip8/defs.h"
#include "logger.h"

int opcode_2(const uint16_t opcode, chip8_system_t *c8)
{
    uint16_t jmp_address = opcode & 0x0FFF;

    logger(LOG_DEBUG, "opcode 0x2NNN: call subroutine at 0x%04X", jmp_address);

    if (c8->stack_ptr + 1 < STACK_SIZE) {
        c8->stack[c8->stack_ptr] = c8->pc; // store the current PC on the stack
        c8->stack_ptr += 1; // increase the stack pointer
    } else {
        // stack is full
        logger(LOG_CRIT, "the stack is full");
        return -1;
    }

    c8->pc = jmp_address; // jump to the subroutine address

    return 0;
}