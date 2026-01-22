#include "display.h"
#include "memory.h"
#include "string.h"
#include <stdbool.h>
#include <stdint.h>

void display_clear(Chip8 *chip8) {
  memset(chip8->display_buffer, 0, WIDTH * HEIGHT);
}

uint8_t display_draw_sprite(Chip8 *chip8, uint8_t x, uint8_t y, uint8_t height,
                            uint16_t address) {
  uint8_t pixel_x = x % WIDTH;
  uint8_t pixel_y = y % HEIGHT;
  uint8_t collision_detected = 0;

  for (int row = 0; row < height; row++) {
    if (pixel_y + row >= HEIGHT)
      break;
    uint8_t row_data = memory_read(chip8, address + row);

    for (int col = 0; col < 8; col++) {
      if (pixel_x + col >= WIDTH)
        break;
      uint8_t sprite_pixel = (row_data >> (7 - col)) & 0x01;
      if (sprite_pixel) {
        uint16_t screen_index = (pixel_y + row) * WIDTH + (pixel_x + col);
        if (chip8->display_buffer[screen_index] == 1)
          collision_detected = 1;
        chip8->display_buffer[screen_index] ^= 1;
      }
    }
  }
  return collision_detected;
}

bool display_get_pixel(Chip8 *chip8, uint8_t x, uint8_t y) {
  if (x >= WIDTH || y >= HEIGHT)
    return false;
  return chip8->display_buffer[y * WIDTH + x] != 0;
}
