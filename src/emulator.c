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
        case SDLK_0: c8->keys[0x0] = state;
            break;
        case SDLK_1: c8->keys[0x1] = state;
            break;
        case SDLK_2: c8->keys[0x2] = state;
            break;
        case SDLK_3: c8->keys[0x3] = state;
            break;
        case SDLK_4: c8->keys[0x4] = state;
            break;
        case SDLK_5: c8->keys[0x5] = state;
            break;
        case SDLK_6: c8->keys[0x6] = state;
            break;
        case SDLK_7: c8->keys[0x7] = state;
            break;
        case SDLK_8: c8->keys[0x8] = state;
            break;
        case SDLK_9: c8->keys[0x9] = state;
            break;
        case SDLK_a: c8->keys[0xA] = state;
            break;
        case SDLK_b: c8->keys[0xB] = state;
            break;
        case SDLK_c: c8->keys[0xC] = state;
            break;
        case SDLK_d: c8->keys[0xD] = state;
            break;
        case SDLK_e: c8->keys[0xE] = state;
            break;
        case SDLK_f: c8->keys[0xF] = state;
            break;
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
    const int pixel_size, const bool step)
{
    char window_title[256];
    chip8_system_t *c8;
    SDL_Window *win;
    SDL_Renderer *ren;
    SDL_AudioSpec audio;

    // Initialize Chip8 system
    c8 = create_chip8_system();
    if (!c8)
        return EXIT_FAILURE;

    load_chip8_fontset(c8);
    if (load_chip8_program(filename, c8) < 0) {
        free(c8);
        return EXIT_FAILURE;
    }

    // Initialize SDL window
    if (create_window(&win, &ren, pixel_size) < 0) {
        free(c8);
        return EXIT_FAILURE;
    }

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
    int break_loop = 0; // If nonzero, break the loop and exit the program
    bool paused = false;
    bool _paused = false;
    uint32_t cycle_nb = 0, last_cycle_nb = 0;
    uint32_t last_tick = 0, last_second = 0, ticks;
    uint32_t interval = cps > 0 ? (uint32_t) 1000 / cps : 0;
    SDL_Event e;

    render_chip8_screen(ren, pixel_size, c8);
    while (!break_loop) {
        ticks = SDL_GetTicks();
        if (!paused && (step || ticks - last_tick >= interval)) {
            last_tick = ticks;

            logger(LOG_DEBUG, "Cycle %u", ++cycle_nb);
            break_loop = cpu_cycle(c8);
            if (c8->screen_refreshed) {
                render_chip8_screen(ren, pixel_size, c8);
            }

            if (c8->sound_timer > 0) {
                SDL_PauseAudio(0);
            } else {
                SDL_PauseAudio(1);
            }

            if (step)
                paused = true;
        }

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT: break_loop = 1;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        break_loop = 1;
                    } else if (e.key.keysym.sym == SDLK_SPACE) {
                        paused = !paused;
                    } else if (e.key.keysym.sym == SDLK_n) {
                        dump_chip8_system(c8);
                    } else {
                        set_key_state(&e, 1, c8);
                    }
                    break;
                case SDL_KEYUP: set_key_state(&e, 0, c8);
                    break;
                default: break;
            }
        }

        if (!paused && ticks - last_second >= 1000) {
            snprintf(
                window_title,
                sizeof(window_title),
                "Chip8 - %s (%u cps)",
                filename,
                cycle_nb - last_cycle_nb);
            SDL_SetWindowTitle(win, window_title);
            last_cycle_nb = cycle_nb;
            last_second = ticks;
        }

        if (paused && !_paused) {
            _paused = paused;
            snprintf(
                window_title,
                sizeof(window_title),
                "Chip8 - %s (paused)",
                filename);
            SDL_SetWindowTitle(win, window_title);
        } else if (!paused && _paused) {
            _paused = paused;
            snprintf(
                window_title,
                sizeof(window_title),
                "Chip8 - %s",
                filename);
            SDL_SetWindowTitle(win, window_title);
        }

        SDL_Delay(interval);
    }

    SDL_CloseAudio();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    free(c8);

    return break_loop >= 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}