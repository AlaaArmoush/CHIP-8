#pragma once
#include "chip8.h"
void cpu_init(Chip8 *chip8);
void cpu_cycle(Chip8 *chip8); // single cycle interpreter
