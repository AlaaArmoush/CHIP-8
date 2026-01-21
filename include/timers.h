#pragma once
#include "chip8.h"
#include <stdint.h>

void timers_update(Chip8 *chip8);
void timers_set_delay(Chip8 *chip8, uint8_t value);
uint8_t timers_get_delay(Chip8 *chip8);
void timers_set_sound(Chip8 *chip8, uint8_t value);
uint8_t timers_get_sound(Chip8 *chip8);
