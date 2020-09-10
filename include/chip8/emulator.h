#include <stdbool.h>

#ifndef _CHIP8_EMULATOR_H
#define _CHIP8_EMULATOR_H

#define EMU_AUDIO_SAMPLE_RATE (22050)

int emulate_chip8_program(const char *filename, const int cps,
    const int pixel_size, const int framerate);

#endif