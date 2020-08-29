#include "chip8/defs.h"
#include <stdbool.h>

#ifndef _CHIP8_SYSTEM_H
#define _CHIP8_SYSTEM_H

chip8_system_t *create_chip8_system(void);
uint16_t load_bytes_in_memory(const byte_t *bytes, const uint16_t length,
    const uint16_t offset, chip8_system_t *c8, const bool set_pc);
int load_chip8_program(const char *filename, chip8_system_t *c8);
uint16_t load_chip8_fontset(chip8_system_t *c8);
void dump_memory(uint16_t address, uint16_t n, chip8_system_t *c8);
void dump_pressed_keys(chip8_system_t *c8);
void dump_stack(chip8_system_t *c8);
void dump_registers(chip8_system_t *c8);
void dump_chip8_system(chip8_system_t *c8);

#endif