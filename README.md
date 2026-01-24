# CHIP-8 Emulator

A CHIP-8 interpreter written in C with SDL2, implementing the full 35-instruction set.

## Prerequisites

- GCC or Clang
- SDL2 development libraries
- Make

## Controls

CHIP-8 uses a hexadecimal keypad (0-F) mapped to your keyboard:
```
CHIP-8 Keypad:        Keyboard Mapping:
┌─┬─┬─┬─┐            ┌─┬─┬─┬─┐
│1│2│3│C│            │1│2│3│4│
├─┼─┼─┼─┤            ├─┼─┼─┼─┤
│4│5│6│D│            │Q│W│E│R│
├─┼─┼─┼─┤     →      ├─┼─┼─┼─┤
│7│8│9│E│            │A│S│D│F│
├─┼─┼─┼─┤            ├─┼─┼─┼─┤
│A│0│B│F│            │Z│X│C│V│
└─┴─┴─┴─┘            └─┴─┴─┴─┘
```

## Build & Run
```bash
# Build
make build

# Run
make run <rom>

# Examples
make run tetris
make run pong
```

<p align="center">
  <img width="48%" height="220" alt="2" src="https://github.com/user-attachments/assets/17c2b03e-a0b1-4b04-a92b-b8aab27998c0" />
  <img width="48%" height="250" alt="1" src="https://github.com/user-attachments/assets/af91bad1-85c3-40e1-a5f8-ea267369109b" />
</p>
