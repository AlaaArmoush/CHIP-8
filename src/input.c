#include "input.h"
#include "chip8.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void input_init(Chip8 *chip8) { memset(chip8->keypad, 0, NUM_KEYS); }

void input_set_key(Chip8 *chip8, uint8_t key, bool pressed) {
  if (key < NUM_KEYS)
    chip8->keypad[key] = pressed ? 1 : 0;
}

bool input_check_key(Chip8 *chip8, uint8_t key) {
  if (key >= NUM_KEYS)
    return false;
  return chip8->keypad[key] != 0;
}
uint8_t input_wait_for_key(const Chip8 *chip8) {
  for (uint8_t i = 0; i < NUM_KEYS; i++) {
    if (chip8->keypad[i])
      return i;
  }
  return -1;
}
