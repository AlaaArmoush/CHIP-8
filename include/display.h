#pragma once
#include "chip8.h"
#include <cstdint>

void display_clear(Chip8 *chip8);
uint8_t display_draw_sprite(Chip8 *chip8, uint8_t x, uint8_t y, uint8_t height,
                            uint16_t address);
bool display_get_pixel(Chip8 *chip8, uint8_t x, uint8_t y);
