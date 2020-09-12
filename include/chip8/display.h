#include "chip8/defs.h"
#include <stdarg.h>
#include <SDL2/SDL.h>

#ifndef _CHIP8_DISPLAY_H
#define _CHIP8_DISPLAY_H

void set_background_rgb(Uint8 r, Uint8 g, Uint8 b);
void set_foreground_rgb(Uint8 r, Uint8 g, Uint8 b);
int set_pixel_size(int size);
void draw_pixel(SDL_Renderer *ren, const int x, const int y);
void render_chip8_screen(SDL_Renderer *ren, chip8_system_t *c8);
int create_window(SDL_Window **win, SDL_Renderer **ren);
void update_window(SDL_Window *win);
void update_window_title(SDL_Window *win, const char *format, ...);

#endif