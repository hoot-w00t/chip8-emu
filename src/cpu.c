#include "chip8/defs.h"
#include "chip8/opcodes.h"
#include "logger.h"

// return values
// -1: an error occured but was handled
// -2: unknown opcode
int execute_opcode(const uint16_t opcode, chip8_system_t *c8)
{
    // TODO: replace switch statement with a function pointer array
    switch (opcode & 0xF000) {
        case 0x0000: return opcode_0(opcode, c8);
        case 0x1000: return opcode_1(opcode, c8);
        case 0x2000: return opcode_2(opcode, c8);
        case 0x3000: return opcode_3(opcode, c8);
        case 0x4000: return opcode_4(opcode, c8);
        case 0x5000: return opcode_5(opcode, c8);
        case 0x6000: return opcode_6(opcode, c8);
        case 0x7000: return opcode_7(opcode, c8);
        case 0x8000: return opcode_8(opcode, c8);
        case 0x9000: return opcode_9(opcode, c8);
        case 0xA000: return opcode_A(opcode, c8);
        case 0xB000: return opcode_B(opcode, c8);
        case 0xC000: return opcode_C(opcode, c8);
        case 0xD000: return opcode_D(opcode, c8);
        case 0xE000: return opcode_E(opcode, c8);
        case 0xF000: return opcode_F(opcode, c8);
    }
    return -2;
}

int cpu_cycle(chip8_system_t *c8)
{
    uint16_t opcode;
    int result;

    // Make sure that the PC is within memory
    if ((uint32_t) (c8->pc + 1) > sizeof(c8->memory)) {
        logger(LOG_CRIT, "PC is out of memory bounds: 0x%04X", c8->pc);
        return -1;
    }

    // Retrieve the opcode
    logger(LOG_DEBUG, "Reading opcode at PC 0x%04X", c8->pc);
    opcode = (c8->memory[c8->pc] << 8) | c8->memory[c8->pc + 1];

    // Decode and execute instruction
    if ((result = execute_opcode(opcode, c8)) < 0) {
        switch (result) {
            case -2: logger(LOG_CRIT, "Unknown opcode 0x%04X", opcode);
                break;
            default: break;
        }

        return result;
    }

    if (c8->sound_timer > 0) {
        c8->sound_timer -= 1;
    }
    if (c8->delay_timer > 0) {
        c8->delay_timer -= 1;
    }
    return 0;
}