#include "audio.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_stdinc.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#define PI 3.14159265358979323846f

static SDL_AudioDeviceID audio_device = 0;
static bool beep_active = false;

static void audio_callback(void *userdata, uint8_t *stream, int len) {
  (void)userdata; // to avoid warnings

  if (!beep_active) {
    memset(stream, 0, (size_t)len);
    return;
  }

  static float wave_phase; // static maintans value between calls

  int16_t *out = (int16_t *)stream;
  int num_samples = len / 2;

  const float beep_frequency = 440.0f;
  const float sample_rate = 44100.0f;
  const float phase_inc = 2.0f * (float)PI * beep_frequency / sample_rate;
  const float amplitude = 3000.0f;

  for (int i = 0; i < num_samples; i++) {
    out[i] = (int16_t)(sinf(wave_phase) * amplitude);
    wave_phase += phase_inc;
    if (wave_phase >= 2.0f * (float)PI)
      wave_phase -= 2.0f * (float)PI;
  }
}

bool audio_init(void) {
  SDL_AudioSpec desired, actual;

  SDL_zero(desired);
  desired.freq = 44100.0;
  desired.format = AUDIO_S16SYS;
  desired.channels = 1;
  desired.samples = 512;
  desired.callback = audio_callback;

  audio_device = SDL_OpenAudioDevice(NULL, 0, &desired, &actual, 0);
  if (audio_device == 0) {
    fprintf(stderr, "Failed to open audio: %s\n", SDL_GetError());
    return false;
  }

  SDL_PauseAudioDevice(audio_device, 0);
  return true;
}
