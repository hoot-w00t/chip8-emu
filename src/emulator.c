#include "chip8/system.h"
#include "chip8/cpu.h"
#include "chip8/audio.h"
#include "chip8/emulator.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

// set *c8->keys[key] state accordingly
void set_key_state(SDL_Event *e, byte_t state, chip8_system_t *c8)
{
    switch (e->key.keysym.sym) {
        case SDLK_0: c8->keys[0x0] = state; if (state) c8->keypress = 0x0; break;
        case SDLK_1: c8->keys[0x1] = state; if (state) c8->keypress = 0x1; break;
        case SDLK_2: c8->keys[0x2] = state; if (state) c8->keypress = 0x2; break;
        case SDLK_3: c8->keys[0x3] = state; if (state) c8->keypress = 0x3; break;
        case SDLK_4: c8->keys[0x4] = state; if (state) c8->keypress = 0x4; break;
        case SDLK_5: c8->keys[0x5] = state; if (state) c8->keypress = 0x5; break;
        case SDLK_6: c8->keys[0x6] = state; if (state) c8->keypress = 0x6; break;
        case SDLK_7: c8->keys[0x7] = state; if (state) c8->keypress = 0x7; break;
        case SDLK_8: c8->keys[0x8] = state; if (state) c8->keypress = 0x8; break;
        case SDLK_9: c8->keys[0x9] = state; if (state) c8->keypress = 0x9; break;
        case SDLK_a: c8->keys[0xA] = state; if (state) c8->keypress = 0xA; break;
        case SDLK_b: c8->keys[0xB] = state; if (state) c8->keypress = 0xB; break;
        case SDLK_c: c8->keys[0xC] = state; if (state) c8->keypress = 0xC; break;
        case SDLK_d: c8->keys[0xD] = state; if (state) c8->keypress = 0xD; break;
        case SDLK_e: c8->keys[0xE] = state; if (state) c8->keypress = 0xE; break;
        case SDLK_f: c8->keys[0xF] = state; if (state) c8->keypress = 0xF; break;
        default: break;
    }
}

// draw a pixel on the screen
void draw_pixel(SDL_Renderer *ren, const int pixel_size,
    const int x, const int y)
{
    SDL_Rect rect;

    rect.h = pixel_size;
    rect.w = pixel_size;
    rect.x = x * pixel_size;
    rect.y = y * pixel_size;

    SDL_RenderFillRect(ren, &rect);
}

// render the *c8->video_memory
void render_chip8_screen(SDL_Renderer *ren, const int pixel_size,
    chip8_system_t *c8)
{
    c8->screen_refreshed = 0;
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            if (c8->video_memory[x + (y * SCREEN_WIDTH)])
                draw_pixel(ren, pixel_size, x, y);
        }
    }

    SDL_RenderPresent(ren);
}

// create SDL window
int create_window(SDL_Window **win, SDL_Renderer **ren, const int pixel_size)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return -1;
    }

    SDL_CreateWindowAndRenderer(
        SCREEN_WIDTH * pixel_size,
        SCREEN_HEIGHT * pixel_size,
        0,
        win,
        ren);

    if (*win == NULL || *ren == NULL) {
        fprintf(stderr, "Unable to create SDL window: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    return 0;
}

// load and emulate *filename at cps cycles per second
int emulate_chip8_program(const char *filename, const int cps,
    const int pixel_size, const int framerate)
{
    int break_loop = 0;
    bool paused = false;
    uint32_t cycles, last_cycle = 0, last_tick = 0, frames = 0;
    double elapsed, refresh = 0, timers = 0;
    char window_title[256];
    SDL_Window *win;
    SDL_Renderer *ren;
    SDL_AudioSpec audio;
    SDL_Event e;
    chip8_system_t *c8;

    // Initialize Chip8 system
    c8 = create_chip8_system();
    if (!c8) return EXIT_FAILURE;

    // Load Chip8 program in memory
    if (load_chip8_program(filename, c8) < 0) {
        free(c8);
        return EXIT_FAILURE;
    }

    // Initialize SDL window
    if (create_window(&win, &ren, pixel_size) < 0) {
        free(c8);
        return EXIT_FAILURE;
    }

    // Set window title
    snprintf(
        window_title,
        sizeof(window_title),
        "Chip8 - %s",
        filename);
    SDL_SetWindowTitle(win, window_title);

    // Initialize audio
    open_audio(&audio);
    SDL_PauseAudio(1);

    // Emulation loop
    render_chip8_screen(ren, pixel_size, c8);
    while (break_loop >= 0) {
        elapsed = (SDL_GetTicks() - last_tick) / 1000.0;
        frames += 1; refresh += elapsed; timers += elapsed;

        if (!paused) {
            cycles = elapsed * cps;

            for (uint32_t i = 0; i < cycles; ++i) break_loop = cpu_cycle(c8);
            if (c8->screen_refreshed) render_chip8_screen(ren, pixel_size, c8);

            for (; timers >= 1.0 / 60.0; timers -= 1.0 / 60.0) {
                if (c8->sound_timer > 0) {
                    SDL_PauseAudio(0);
                    c8->sound_timer -= 1;
                } else {
                    SDL_PauseAudio(1);
                }
                if (c8->delay_timer > 0) {
                    c8->delay_timer -= 1;
                }
            }
        }

        last_tick = SDL_GetTicks();

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: break_loop = -2; break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        break_loop = -2;
                    } else if (e.key.keysym.sym == SDLK_SPACE) {
                        paused = !paused;
                    } else if (e.key.keysym.sym == SDLK_n) {
                        dump_chip8_system(c8);
                    } else {
                        set_key_state(&e, 1, c8);
                    }
                    break;

                case SDL_KEYUP: set_key_state(&e, 0, c8); break;
                default: break;
            }
        }

        if (refresh >= 1) {
            if (paused) {
                snprintf(
                    window_title,
                    sizeof(window_title),
                    "Chip8 - %s (paused)",
                    filename);
            } else {
                snprintf(
                    window_title,
                    sizeof(window_title),
                    "Chip8 - %s (%u cps, %u fps)",
                    filename,
                    c8->cycle - last_cycle,
                    frames);
            }

            SDL_SetWindowTitle(win, window_title);
            last_cycle = c8->cycle; frames = 0; refresh = 0;
        }

        SDL_Delay(1000 / framerate);
    }

    SDL_CloseAudio();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    free(c8);

    return 0;
}