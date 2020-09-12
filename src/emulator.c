#include "chip8/system.h"
#include "chip8/cpu.h"
#include "chip8/audio.h"
#include "chip8/display.h"
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

// load and emulate *filename at cps cycles per second
int emulate_chip8_program(const char *filename, const int cps, const int framerate)
{
    int break_loop = 0;
    bool paused = false;
    uint32_t cycles, last_cycle = 0, last_tick = 0, frames = 0;
    double elapsed, refresh = 0, timers = 0;
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
    if (create_window(&win, &ren) < 0) {
        free(c8);
        return EXIT_FAILURE;
    }

    // Set window title
    update_window_title(win, "Chip8 - %s", filename);

    // Initialize audio
    open_audio(&audio);
    SDL_PauseAudio(1);

    // Emulation loop
    render_chip8_screen(ren, c8);
    while (break_loop >= 0) {
        elapsed = (SDL_GetTicks() - last_tick) / 1000.0;
        frames += 1; refresh += elapsed; timers += elapsed;

        if (!paused) {
            cycles = elapsed * cps;

            for (uint32_t i = 0; i < cycles; ++i) break_loop = cpu_cycle(c8);
            if (c8->screen_refreshed) render_chip8_screen(ren, c8);

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
                        if (paused) update_window_title(win, "Chip8 - %s (paused)", filename);
                        else update_window_title(win, "Chip8 - %s", filename);
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
                update_window_title(win, "Chip8 - %s (paused)", filename);
            } else {
                update_window_title(
                    win,
                    "Chip8 - %s (%u cps, %u fps)",
                    filename,
                    c8->cycle - last_cycle,
                    frames);
            }

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