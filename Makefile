CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Iinclude -fsyntax-only

HEADERS := \
	include/audio.h \
	include/chip8.h \
	include/cpu.h \
	include/display.h \
	include/input.h \
	include/memory.h \
	include/timers.h

.PHONY: headers

headers:
	$(CC) $(CFLAGS) $(HEADERS)
