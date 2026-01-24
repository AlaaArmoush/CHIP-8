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

  case 0x2000:
    // 2NNN: Call subroutine at address NNN
    chip8->stack[chip8->stack_pointer++] =
        chip8->program_counter; // save addr to return to
    chip8->program_counter = address_nnn;
    break;

  case 0x3000:
    // 3XNN: Skip next instruction if VX == NN
    if (chip8->registers[x_reg] == immediate_nn) {
      chip8->program_counter += 2;
    }
    break;

  case 0x4000:
    // 4XNN: skip next instruction if VX != NN
    if (chip8->registers[x_reg] != immediate_nn) {
      chip8->program_counter += 2;
    }
    break;

  case 0x5000:
    // 5XY0: skip next instruction if VX == VY
    if (chip8->registers[x_reg] == chip8->registers[y_reg]) {
      chip8->program_counter += 2;
    }
    break;

  case 0x9000:
    // 9XY0: skip next instruction if VX != VY
    if (chip8->registers[x_reg] != chip8->registers[y_reg]) {
      chip8->program_counter += 2;
    }
    break;

  case 0x8000:
    switch (nibble_n) {
    case 0x0:
      chip8->registers[x_reg] = chip8->registers[y_reg];
      break;
    case 0x1:
      chip8->registers[x_reg] |= chip8->registers[y_reg];
      break;
    case 0x2:
      chip8->registers[x_reg] &= chip8->registers[y_reg];
      break;
    case 0x3:
      chip8->registers[x_reg] ^= chip8->registers[y_reg];
      break;
    case 0x4:;
      // unlike 7XNN thr carry flag is affected here
      uint16_t sum = chip8->registers[x_reg] + chip8->registers[y_reg];
      chip8->registers[0xF] = (sum > 255) ? 1 : 0;
      chip8->registers[x_reg] = sum & 0xFF;
      break;
    case 0x5:
      chip8->registers[0xF] =
          (chip8->registers[x_reg] >= chip8->registers[y_reg]) ? 1 : 0;
      chip8->registers[x_reg] -= chip8->registers[y_reg];
      break;
    case 0x07:
      chip8->registers[0xF] =
          (chip8->registers[x_reg] <= chip8->registers[y_reg]) ? 1 : 0;
      chip8->registers[x_reg] =
          chip8->registers[y_reg] - chip8->registers[x_reg];
      break;
    case 0x06:
      chip8->registers[x_reg] = chip8->registers[y_reg];
      chip8->registers[x_reg] >>= 1;
      break;
    case 0x0E:
      chip8->registers[x_reg] = chip8->registers[y_reg];
      chip8->registers[x_reg] <<= 1;
      break;
    }
    break;

  case 0xB000:
    chip8->program_counter = address_nnn + chip8->registers[0x0];
    break;

  case 0xC000:
    chip8->registers[x_reg] = (rand() % 256) & immediate_nn;
    break;

  case 0xE000:
    if (immediate_nn == 0x9E) {
      if (input_check_key(chip8, chip8->registers[x_reg])) {
        chip8->program_counter += 2;
      }
    } else if (immediate_nn == 0xA1) {
      if (!input_check_key(chip8, chip8->registers[x_reg])) {
        chip8->program_counter += 2;
      }
    }
    break;

  case 0xF000:
    switch (immediate_nn) {
    case 0x07:
      chip8->registers[x_reg] = timers_get_delay(chip8);
      break;
    case 0x15:
      timers_set_delay(chip8, chip8->registers[x_reg]);
      break;
    case 0x18:
      timers_set_sound(chip8, chip8->registers[x_reg]);
      break;
    case 0x1E:
      chip8->index_register += chip8->registers[x_reg];
      break;
    case 0x0A: {
      int8_t pressed_key = input_wait_for_key(chip8);
      if (pressed_key != -1) {
        chip8->registers[x_reg] = pressed_key;
      } else {
        // repeat instruction
        chip8->program_counter -= 2;
      }
      break;
    }
    case 0x29:
      chip8->index_register = FONT_START + chip8->registers[x_reg] * 5;
      break;
    case 0x33:;
      uint8_t value = chip8->registers[x_reg];
      memory_write(chip8, chip8->index_register, value / 100);
      memory_write(chip8, chip8->index_register + 1, (value / 10) % 10);
      memory_write(chip8, chip8->index_register + 2, value % 10);
      break;
    case 0x55:
      for (uint8_t i = 0; i <= x_reg; i++) {
        memory_write(chip8, chip8->index_register + i, chip8->registers[i]);
      }
      break;
    case 0x65:
      for (int i = 0; i <= x_reg; i++) {
        chip8->registers[i] = memory_read(chip8, chip8->index_register + i);
      }
      break;
    }
    break;
  default:
    printf("Unknown opcode: 0x%04X\n", opcode);
    break;
  }
}
