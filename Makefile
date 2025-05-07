CC ?= clang
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700L -D_POSIX_C_SOURCE=200809L
CFLAGS += -std=c99 -Wall -Wextra -ggdb -O2 -Iinclude
LDFLAGS +=
SRC_DIR = src
# TEST_DIR = test
BUILD_DIR = build
MKDIR = mkdir -p

SHARED_LIB_EXT = so
SHARED_LIB_FLAGS = -shared -fPIC

SRC_FILES = \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/bignum.c \
	$(SRC_DIR)/bignum_bitwise.c \
	$(SRC_DIR)/bignum_add_sub.c \
#	$(SRC_DIR)/bignum_div.c

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

TEST_BIN = $(BUILD_DIR)/main
LIB_NAME = libbignum
LIB_PATH = $(BUILD_DIR)/$(LIB_NAME).$(SHARED_LIB_EXT)

.PHONY: all release test clean build_dir

all: test

test: $(TEST_BIN)

release: $(LIB_PATH)

build_dir:
	mkdir -p $(BUILD_DIR)

$(OBJ_FILES): build_dir

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(TEST_BIN): main.c $(OBJ_FILES)
	$(CC) $(CPPFLAGS) main.c $(OBJ_FILES) $(CFLAGS) -o $@ $(LDFLAGS)

$(LIB_PATH): $(OBJ_FILES)
	$(CC) $(CPPFLAGS) $(OBJ_FILES) $(SHARED_LIB_FLAGS) -o $@ $(LDFLAGS)

clean:
	rm -rf  $(BUILD_DIR)
