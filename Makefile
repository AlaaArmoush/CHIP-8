CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Iinclude -fsyntax-only

HEADERS := $(wildcard include/*.h)
SOURCES := $(wildcard src/*.c)

.PHONY: headers modules all

all: headers modules

headers:
	$(CC) $(CFLAGS) $(HEADERS)

modules:
	$(CC) $(CFLAGS) $(SOURCES)

