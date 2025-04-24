CC ?= clang
CFLAGS += -Wall -Wextra -O2 -Iinclude
LDFLAGS +=
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build
MKDIR = mkdir -p
RM = rm -rf

SHARED_LIB_EXT = so
SHARED_LIB_FLAGS = -shared -fPIC

SRC_FILES = \
	$(SRC_DIR)/bignum.c
#	$(SRC_DIR)/bignum_add.c \
#	$(SRC_DIR)/bignum_mul.c \
#	$(SRC_DIR)/bignum_div.c

OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

TEST_BIN = $(BUILD_DIR)/test
LIB_NAME = libbignum
LIB_PATH = $(BUILD_DIR)/$(LIB_NAME).$(SHARED_LIB_EXT)

.PHONY: all release test clean build_dir

all: test

test: $(TEST_BIN)

release: $(LIB_PATH)

build_dir:
	$(MKDIR) $(BUILD_DIR)

$(OBJ_FILES): build_dir

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_DIR)/main.c $(OBJ_FILES)
	$(CC) $(TEST_DIR)/main.c $(OBJ_FILES) $(CFLAGS) -o $@ $(LDFLAGS)

$(LIB_PATH): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(SHARED_LIB_FLAGS) -o $@ $(LDFLAGS)

clean:
	$(RM) $(BUILD_DIR)
