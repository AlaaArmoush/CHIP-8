#pragma once
#include "chip8.h"
#include <stdbool.h>
#include <stdint.h>

void input_init(Chip8 *chip8);
void input_set_key(Chip8 *chip8, uint8_t key, bool pressed);
bool input_check_key(Chip8 *chip8, uint8_t key);
uint8_t input_wait_for_key(Chip8 *chip8);
