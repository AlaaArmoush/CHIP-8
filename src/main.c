#include "audio.h"
#include "chip8.h"
#include "cpu.h"
#include "display.h"
#include "input.h"
#include "memory.h"
#include "timers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define PIXEL_SCALE 10
#define WINDOW_WIDTH (WIDTH * PIXEL_SCALE)
#define WINDOW_HEIGHT (HEIGHT * PIXEL_SCALE)

static const SDL_Scancode chip8_keymap[NUM_KEYS] = {
    SDL_SCANCODE_1, // 1
    SDL_SCANCODE_2, // 2
    SDL_SCANCODE_3, // 3
    SDL_SCANCODE_4, // C

    SDL_SCANCODE_Q, // 4
    SDL_SCANCODE_W, // 5
    SDL_SCANCODE_E, // 6
    SDL_SCANCODE_R, // D

    SDL_SCANCODE_A, // 7
    SDL_SCANCODE_S, // 8
    SDL_SCANCODE_D, // 9
    SDL_SCANCODE_F, // E

    SDL_SCANCODE_Z, // A
    SDL_SCANCODE_X, // 0
    SDL_SCANCODE_C, // B
    SDL_SCANCODE_V  // F
};

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <ROM file>\n", argv[0]);
    return 1;
  }

  Chip8 chip8;
  memset(&chip8, 0, sizeof(Chip8));

  memory_init(&chip8);
  cpu_init(&chip8);
  input_init(&chip8);
  memory_load_rom(&chip8, argv[1]);

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  SDL_SetWindowSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);

  if (!window) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  if (!audio_init()) {
    fprintf(stderr, "Warning: Audio initialization failed\n");
  }

  bool quit = false;
  SDL_Event event;

  uint64_t last_timer_update = SDL_GetTicks64();

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        quit = true;
    }

    const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
    for (uint8_t i = 0; i < NUM_KEYS; i++) {
      input_set_key(&chip8, i, keyboard_state[chip8_keymap[i]]);
    }

    // 10 instruction per frame (600 per second)
    for (uint8_t i = 0; i < 10; i++) {
      cpu_cycle(&chip8);
    }

    uint64_t current_time = SDL_GetTicks64();
    if (current_time - last_timer_update >= 16) {
      timers_update(&chip8);
      last_timer_update = current_time;
      if (timers_get_sound(&chip8) > 0) {
        audio_start_beep();
      } else {
        audio_stop_beep();
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int y = 0; y < HEIGHT; y++) {
      for (int x = 0; x < WIDTH; x++) {
        if (display_get_pixel(&chip8, x, y)) {
          SDL_Rect rect = {x * PIXEL_SCALE, y * PIXEL_SCALE, PIXEL_SCALE,
                           PIXEL_SCALE};
          SDL_RenderFillRect(renderer, &rect);
        }
      }
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
  }

  audio_cleanup();
  SDL_RenderClear(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
