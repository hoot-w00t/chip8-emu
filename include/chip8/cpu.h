#ifndef _CHIP8_CPU_H
#define _CHIP8_CPU_H

int execute_opcode(const uint16_t opcode, chip8_system_t *c8);
int cpu_cycle(chip8_system_t *c8);

#endif