#include "chip8/defs.h"
#include <stdarg.h>
#include <SDL2/SDL.h>

static int pixel_size = 8;
static Uint8 bg[3] = {0, 0, 0};
static Uint8 fg[3] = {255, 255, 255};

// set the background color
void set_background_rgb(Uint8 r, Uint8 g, Uint8 b)
{
    bg[0] = r;
    bg[1] = g;
    bg[2] = b;
}

// set the foreground color
void set_foreground_rgb(Uint8 r, Uint8 g, Uint8 b)
{
    fg[0] = r;
    fg[1] = g;
    fg[2] = b;
}

// set the pixel size
// return -1 on error
int set_pixel_size(int size)
{
    if (size > 0) {
        pixel_size = size;
        return 0;
    }
    return -1;
}

// draw a pixel on the screen
void draw_pixel(SDL_Renderer *ren, const int x, const int y)
{
    SDL_Rect rect;

    rect.h = pixel_size;
    rect.w = pixel_size;
    rect.x = x * pixel_size;
    rect.y = y * pixel_size;

    SDL_RenderFillRect(ren, &rect);
}

// render the *c8->video_memory
void render_chip8_screen(SDL_Renderer *ren, chip8_system_t *c8)
{
    c8->screen_refreshed = 0;
    SDL_SetRenderDrawColor(ren, bg[0], bg[1], bg[2], 255);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, fg[0], fg[1], fg[2], 255);

    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            if (c8->video_memory[x + (y * SCREEN_WIDTH)])
                draw_pixel(ren, x, y);
        }
    }

    SDL_RenderPresent(ren);
}

// create SDL window
int create_window(SDL_Window **win, SDL_Renderer **ren)
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

// update SDL window size
void update_window(SDL_Window *win)
{
    SDL_SetWindowSize(
        win,
        SCREEN_WIDTH * pixel_size,
        SCREEN_HEIGHT * pixel_size);
}

// update SDL window title
void update_window_title(SDL_Window *win, const char *format, ...)
{
    char title[256];
    va_list ap;

    va_start(ap, format);
    vsnprintf(
        title,
        sizeof(title),
        format,
        ap);
    va_end(ap);

    SDL_SetWindowTitle(win, title);
}