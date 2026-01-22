#include "timers.h"
#include <stdint.h>

void timers_update(Chip8 *chip8) {
  if (chip8->delay_timer > 0)
    chip8->delay_timer--;
  if (chip8->sound_timer > 0)
    chip8->sound_timer--;
}

void timers_set_delay(Chip8 *chip8, uint8_t value) {
  chip8->delay_timer = value;
}

uint8_t timers_get_delay(Chip8 *chip8) { return chip8->delay_timer; }

void timers_set_sound(Chip8 *chip8, uint8_t value) {
  chip8->sound_timer = value;
}

uint8_t timers_get_sound(Chip8 *chip8) { return chip8->sound_timer; }
