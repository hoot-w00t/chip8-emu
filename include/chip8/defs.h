#include <stdint.h>

#ifndef _CHIP8_DEFS_H
#define _CHIP8_DEFS_H

#define LOAD_ADDRESS (0x200)
#define MEMORY_SIZE (4096)
#define REGISTER_COUNT (16)
#define STACK_SIZE (16)
#define SCREEN_WIDTH (64)
#define SCREEN_HEIGHT (32)

typedef uint8_t byte_t;
typedef struct chip8_system chip8_system_t;

/* Memory map

    0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    0x200-0xFFF - Program ROM and work RAM
*/

struct chip8_system {
    byte_t V[REGISTER_COUNT];     // registers (from V0 to VF)
    uint16_t I;                   // address register
    uint16_t pc;                  // program counter
    byte_t memory[MEMORY_SIZE];   // memory
    byte_t video_memory[SCREEN_WIDTH * SCREEN_HEIGHT]; // video memory
    byte_t delay_timer;           // delay timer (for events, read/write)
    byte_t sound_timer;           // sound timer (buzzer)
    byte_t keys[16];              // hex keys (stores states for each key)
    uint16_t stack[STACK_SIZE];   // stack
    uint16_t stack_ptr;           // stack pointer
    byte_t screen_refreshed;      // 0 if screen doesn't need a refresh, 1 if it does
};

#endif