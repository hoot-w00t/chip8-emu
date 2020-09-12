#include "chip8/defs.h"
#include "logger.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

const byte_t chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// load length amount of bytes in c8->memory starting at offset
// if we reach the end of the memory space, discard the remaining bytes
// return the amount of written bytes
uint16_t load_bytes_in_memory(const byte_t *bytes, const uint16_t length,
    const uint16_t offset, chip8_system_t *c8, const bool set_pc)
{
    uint16_t i, j;

    if (set_pc)
        c8->pc = offset;

    logger(LOG_DEBUG, "Loading %u bytes in memory starting at address 0x%04X%s",
                      length,
                      offset,
                      set_pc ? " (set to PC)" : "");

    for (i = 0, j = offset; i < length; ++i, ++j) {
        if (j >= sizeof(c8->memory) - 1)
            break;

        c8->memory[j] = bytes[i];
    }

    return i;
}

// load a chip8 program
// return the amount of loaded bytes
int load_chip8_program(const char *filename, chip8_system_t *c8)
{
    #if defined(WIN32)
    const int oflag = O_RDONLY | O_BINARY;
    #else
    const int oflag = O_RDONLY;
    #endif

    byte_t buffer[sizeof(c8->memory)];
    uint16_t size;
    int fd, n;

    if ((fd = open(filename, oflag)) < 0) {
        logger(LOG_ERROR, "Unable to open: %s: %s",
                          filename,
                          strerror(errno));
        return -1;
    }

    size = 0;
    while ((n = read(fd, &buffer[size], sizeof(c8->memory) - size)) > 0) {
        size += n;
    }
    close(fd);

    logger(LOG_DEBUG, "Read %u bytes from file: %s",
                      size,
                      filename);

    if (n < 0) {
        logger(LOG_ERROR, "Error while reading: %s: %s",
                          filename,
                          strerror(errno));
        return -2;
    }

    return load_bytes_in_memory(buffer, size, LOAD_ADDRESS, c8, true);
}

// load fontset at address 0x0
uint16_t load_chip8_fontset(chip8_system_t *c8)
{
    return load_bytes_in_memory(
        chip8_fontset,
        sizeof(chip8_fontset),
        0x0,
        c8,
        false);
}

// reset a chip8 system
void reset_chip8_system(chip8_system_t *c8)
{
    memset(c8, 0, sizeof(chip8_system_t));

    c8->pc = LOAD_ADDRESS;
    c8->keypress_reg = 0xFF;
    c8->keypress = 0xFF;
    c8->screen_refreshed = 1;

    load_chip8_fontset(c8);
}

// allocate a chip8 system and initialize it
chip8_system_t *create_chip8_system(void)
{
    chip8_system_t *c8 = malloc(sizeof(chip8_system_t));

    if (!c8) return NULL;

    reset_chip8_system(c8);
    return c8;
}

// dump n bytes starting at address from c8->memory
void dump_memory(uint16_t address, uint16_t n, chip8_system_t *c8)
{
    uint16_t bytes = 0;

    for (uint16_t i = address; bytes < n; i += 16) {
        printf("0x%04X   ", i);
        for (uint16_t j = 0; j < 16 && bytes < n; ++i, ++bytes) {
            printf("%02X%c", c8->memory[i + j], j + 1 < 16 ? ' ' : '\0');
        }
        printf("\n");
    }
}

// dump pressed keys to stdout
void dump_pressed_keys(chip8_system_t *c8)
{
    printf("Keys:\n");
    for (byte_t i = 0; i < sizeof(c8->keys); ++i) {
        if (c8->keys[i])
            printf("    Key %X\n", i);
    }
}

// dump stack to stdout
void dump_stack(chip8_system_t *c8)
{
    printf("\nStack (pointer at index %u):\n", c8->stack_ptr);
    for (int16_t i = 0; i < STACK_SIZE; ++i) {
        printf("    Stack slot %i: 0x%04X\n", i, c8->stack[i]);
    }
}

// dump registers to stdout
void dump_registers(chip8_system_t *c8)
{
    printf("Registers:\n");
    for (byte_t i = 0; i < sizeof(c8->V); ++i) {
        printf("    V%X: 0x%02X\n", i, c8->V[i]);
    }
}

// dump chip8 system to stdout
void dump_chip8_system(chip8_system_t *c8)
{
    printf("\nPC: 0x%04X    I: 0x%04X\n", c8->pc, c8->I);
    printf("Delay timer: %u    Sound timer: %u\n", c8->delay_timer, c8->sound_timer);
    printf("Last key pressed: %X%s\n",
            c8->keypress,
            c8->keypress_reg <= 0xF ? " (waiting for keypress)" : "");

    dump_registers(c8);
    dump_stack(c8);
    dump_pressed_keys(c8);
}
