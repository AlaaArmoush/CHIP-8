#include "cpu.h"
#include "chip8.h"
#include "display.h"
#include "input.h"
#include "memory.h"
#include "timers.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void execute_instruction(Chip8 *chip8, uint16_t opcode);

void cpu_init(Chip8 *chip8) {
  memset(chip8->registers, 0, NUM_REGISTERS);
  chip8->index_register = 0;
  chip8->program_counter = PROGRAM_START;
  chip8->stack_pointer = 0;
  memset(chip8->stack, 0, STACK_SIZE * sizeof(uint16_t));
  srand(time(NULL));
}

void cpu_cycle(Chip8 *chip8) {
  uint16_t high_byte = memory_read(chip8, chip8->program_counter);
  uint16_t low_byte = memory_read(chip8, chip8->program_counter + 1);
  uint16_t opcode = (high_byte << 8) | low_byte;
  chip8->program_counter += 2;
  execute_instruction(chip8, opcode);
}

static void execute_instruction(Chip8 *chip8, uint16_t opcode) {
  uint8_t x_reg = (opcode >> 8) & 0x0F;
  uint8_t y_reg = (opcode >> 4) & 0x0F;
  uint8_t nibble_n = opcode & 0x0F;
  uint8_t immediate_nn = opcode & 0xFF;
  uint16_t address_nnn = opcode & 0xFFF;

  switch (opcode & 0xF000) {
  case 0x0000:
    if (opcode == 0x00E0) {
      display_clear(chip8);
    } else if (opcode == 0x00EE) {
      // return from subroutine
      chip8->program_counter = chip8->stack[--chip8->stack_pointer];
    }
    break;

  case 0x1000:
    // jump to address 1NNNN
    chip8->program_counter = address_nnn;
    break;

  case 0x6000:
    // 6XNN: set register VX = NN
    chip8->registers[x_reg] = immediate_nn;
    break;

  case 0x7000:
    // 7XNN: Add NN to register VX
    chip8->registers[x_reg] += immediate_nn;
    break;

  case 0xA000:
    // ANNN: set index register = NNN
    chip8->index_register = address_nnn;
    break;

  case 0xD000:
    // DXYN: Draw N-byte sprite from memory location I at (VX, VY)
    chip8->registers[0xF] = display_draw_sprite(
        chip8, chip8->registers[x_reg], chip8->registers[y_reg], nibble_n,
        chip8->index_register);
    break;

  default:
    printf("Unknown opcode: 0x%04X\n", opcode);
    break;
  }
}
