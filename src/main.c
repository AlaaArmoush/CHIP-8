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
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PIXEL_SCALE 10
#define WINDOW_WIDTH (WIDTH * PIXEL_SCALE)
#define WINDOW_HEIGHT (HEIGHT * PIXEL_SCALE)

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

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

  bool quit = false;
  SDL_Event event;

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        quit = true;
    }

    // 10 instruction per frame (600 per second)
    for (uint8_t i = 0; i < 10; i++) {
      cpu_cycle(&chip8);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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

  SDL_RenderClear(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
