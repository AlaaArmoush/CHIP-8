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

SOURCES := \
	src/audio.c \
	src/chip8.c \
	src/display.c \
	src/input.c \
	src/memory.c \
	src/timers.c

.PHONY: headers modules all

all: headers modules

headers:
	$(CC) $(CFLAGS) $(HEADERS)

modules:
	$(CC) $(CFLAGS) $(SOURCES)

