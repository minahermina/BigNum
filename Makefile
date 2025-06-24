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

SRC_FILES   := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES   := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

all: test
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

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean build_dir release test all
