#pragma once
#include "chip8.h"
#include <cstdint>

void memory_init(Chip8 *chip8);
void memory_load_rom(Chip8 *chip8, const char *filename);
uint8_t memory_read(Chip8 *chip8, uint16_t address);
void memory_write(Chip8 *chip8, uint16_t address, uint8_t value);
