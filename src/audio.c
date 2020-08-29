#include "chip8/emulator.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_audio.h>

// audio beep
void audio_buzzer(__attribute__((unused)) void *userdata, Uint8 *stream, int len)
{
    const Sint16 amplitude = 32000;
    const float wave_freq = 1000.0;
    const uint32_t interval = EMU_AUDIO_SAMPLE_RATE / wave_freq;
    Sint16 *buf = (Sint16 *) stream;
    int buflen = len / 2;
    uint32_t sample_nb = 0;
    bool high = true;

    for(int i = 0; i < buflen; ++i, ++sample_nb)
    {
        if (sample_nb >= interval) {
            sample_nb = 0;
            high = !high;
        }

        if (high) {
            buf[i] = amplitude;
        } else {
            buf[i] = 0;
        }
    }
}

// open default audio device
int open_audio(SDL_AudioSpec *audio)
{
    audio->freq = EMU_AUDIO_SAMPLE_RATE;
    audio->format = AUDIO_S16SYS;
    audio->channels = 1;
    audio->samples = 2048;
    audio->callback = audio_buzzer;
    audio->userdata = NULL;

    if (SDL_OpenAudio(audio, NULL) < 0) {
        fprintf(stderr, "Unable to open audio device: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}