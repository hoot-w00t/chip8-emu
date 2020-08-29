#include "chip8/defs.h"

#ifndef _CHIP8_OPCODES_H
#define _CHIP8_OPCODES_H

int opcode_0(const uint16_t opcode, chip8_system_t *c8);
int opcode_1(const uint16_t opcode, chip8_system_t *c8);
int opcode_2(const uint16_t opcode, chip8_system_t *c8);
int opcode_3(const uint16_t opcode, chip8_system_t *c8);
int opcode_4(const uint16_t opcode, chip8_system_t *c8);
int opcode_5(const uint16_t opcode, chip8_system_t *c8);
int opcode_6(const uint16_t opcode, chip8_system_t *c8);
int opcode_7(const uint16_t opcode, chip8_system_t *c8);
int opcode_8(const uint16_t opcode, chip8_system_t *c8);
int opcode_9(const uint16_t opcode, chip8_system_t *c8);
int opcode_A(const uint16_t opcode, chip8_system_t *c8);
int opcode_B(const uint16_t opcode, chip8_system_t *c8);
int opcode_C(const uint16_t opcode, chip8_system_t *c8);
int opcode_D(const uint16_t opcode, chip8_system_t *c8);
int opcode_E(const uint16_t opcode, chip8_system_t *c8);
int opcode_F(const uint16_t opcode, chip8_system_t *c8);

#endif