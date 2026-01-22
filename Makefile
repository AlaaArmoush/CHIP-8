CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS := -lSDL2 -lm

TARGET := chip8
BUILD_DIR := build
BIN_DIR := bin

HEADERS := $(wildcard include/*.h)
SOURCES := $(wildcard src/*.c)
OBJECTS := $(SOURCES:src/%.c=$(BUILD_DIR)/%.o)

.PHONY: all headers modules build run clean dirs

all: headers modules

headers:
	$(CC) $(CFLAGS) -fsyntax-only $(HEADERS)

modules:
	$(CC) $(CFLAGS) -fsyntax-only $(SOURCES)

# Create directories if needed
dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Compile .c → build/*.o
$(BUILD_DIR)/%.o: src/%.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable → bin/chip8
build: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Run: make run PONG
ROM_NAME := $(word 2, $(MAKECMDGOALS))
ROM_PATH := roms/$(ROM_NAME).ch8

run: build
	@if [ -z "$(ROM_NAME)" ]; then \
		echo "Usage: make run <rom_name>"; \
		exit 1; \
	fi
	@if [ ! -f "$(ROM_PATH)" ]; then \
		echo "ROM not found: $(ROM_PATH)"; \
		exit 1; \
	fi
	./$(BIN_DIR)/$(TARGET) "$(ROM_PATH)"

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

%:
	@:

