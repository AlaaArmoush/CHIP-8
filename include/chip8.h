#pragma once
#include <stdint.h>

#define WIDTH 64
#define HEIGHT 32
#define MEMORY_SIZE 4096
#define PROGRAM_START 0x200
#define FONT_START 0x50
#define STACK_SIZE 16
#define NUM_REGISTERS 16
#define NUM_KEYS 16

typedef struct {
  uint8_t memory[MEMORY_SIZE];
  uint8_t registers[NUM_REGISTERS];
  uint16_t index_register;
  uint16_t program_counter;
  uint8_t stack_pointer;
  uint16_t stack[STACK_SIZE];
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint8_t display_buffer[WIDTH * HEIGHT];
  uint8_t keypad[NUM_KEYS];
} Chip8;
