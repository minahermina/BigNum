include config.mk

CC       ?= c99
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -D_POSIX_C_SOURCE=200809L
CFLAGS   = -std=c99 -Wall -Wextra -fPIC -ggdb -O3 -Iinclude
LDFLAGS  =

SRC_DIR     := src
BUILD_DIR   := build
INCLUDE_DIR := include

LIB_NAME    := libbignum
LIB_PATH    := $(BUILD_DIR)/$(LIB_NAME).so
TEST_BIN    := $(BUILD_DIR)/main

# Dependency files
ARENA_SRC   := $(SRC_DIR)/arena.c
ARENA_H     := $(INCLUDE_DIR)/arena.h
ARENA_OBJ   := $(BUILD_DIR)/arena.o

SRC_FILES   := $(filter-out $(ARENA_SRC), $(wildcard $(SRC_DIR)/*.c))
OBJ_FILES   := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES)) $(ARENA_OBJ)

all: deps test

deps:
	@if [ ! -f "$(ARENA_H)" ] || \
		[ ! -f "$(ARENA_SRC)" ] ; then \
		echo "==> Dependencies not found, fetching..."; \
		$(MAKE) fetch; \
	fi

fetch:
	@echo "==> Fetching dependencies..."
	@if [ ! -d "$(SRC_DIR)" ]; then mkdir -p $(SRC_DIR); fi
	@if [ ! -d "$(INCLUDE_DIR)" ]; then mkdir -p $(INCLUDE_DIR); fi
	curl -L "$(ARENA_URL)/arena.c" -o $(ARENA_SRC) || \
		{ echo "Failed to download arena.c"; exit 1; }
	curl -L "$(ARENA_URL)/arena.h" -o $(ARENA_H) || \
		{ echo "Failed to download arena.h"; exit 1; }
	@echo "==> Dependencies fetched successfully"

test: $(TEST_BIN)
release: $(LIB_PATH)

build_dir:
	@mkdir -p $(BUILD_DIR)

# main.c test file
$(TEST_BIN): main.c $(LIB_PATH) | build_dir
	$(CC) $(CPPFLAGS) $(CFLAGS) main.c -L$(BUILD_DIR) -lbignum -o $@ $(LDFLAGS)

# shared library
$(LIB_PATH): $(OBJ_FILES) | build_dir
	$(CC) $(CPPFLAGS) $(OBJ_FILES) -shared -fPIC -o $@ $(LDFLAGS)

# compile each object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | build_dir
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Rule for compiling arena since it depends on arena.h specifically
$(ARENA_OBJ): $(ARENA_SRC) $(ARENA_H) | build_dir
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(ARENA_SRC) -o $(ARENA_OBJ)

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(ARENA_SRC) $(ARENA_H)

.PHONY: clean build_dir release test all deps fetch

