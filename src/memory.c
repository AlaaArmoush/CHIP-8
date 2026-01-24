#include "memory.h"
#include "chip8.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t FONT[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void memory_init(Chip8 *chip8) {
  memset(chip8->memory, 0, MEMORY_SIZE);
  memcpy(&chip8->memory[FONT_START], FONT, sizeof(FONT));
}

void memory_load_rom(Chip8 *chip8, const char *filename) {
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Failed to Open ROM: %s\n", filename);
    exit(1);
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);
  if (size > MEMORY_SIZE - PROGRAM_START) {
    fprintf(stderr, "ROM exceeds size limit: %ld bytes (max: %d bytes)\n", size,
            MEMORY_SIZE - PROGRAM_START);
    fclose(file);
    exit(1);
  }

  fread(&chip8->memory[PROGRAM_START], 1, size, file);
  fclose(file);
  printf("Loaded ROM: %s\n", filename);
}

uint8_t memory_read(Chip8 *chip8, uint16_t address) {
  if (address >= MEMORY_SIZE) {
    fprintf(stderr, "Memory read out of bounds: 0x%04X\n", address);
    exit(1);
  }
  return chip8->memory[address];
}

void memory_write(Chip8 *chip8, uint16_t address, uint8_t value) {
  if (address >= MEMORY_SIZE) {
    fprintf(stderr, "Memory write out of bounds: 0x%04X\n", address);
    exit(1);
  }
  chip8->memory[address] = value;
}
