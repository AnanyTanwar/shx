NAME := shell

SRC_DIR := src
INC_DIR := src/include
BUILD_DIR := build
BIN_DIR := bin
TEST_DIR := tests

CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -I$(INC_DIR)
DEBUG_FLAGS := -g -O0 -DDEBUG
RELEASE_FLAGS := -O2
SANITIZE_FLAGS := -fsanitize=address,undefined -g

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

.DEFAULT_GOAL := help

debug: CFLAGS += $(DEBUG_FLAGS)
debug: prepare $(BIN_DIR)/$(NAME)

release: CFLAGS += $(RELEASE_FLAGS)
release: prepare $(BIN_DIR)/$(NAME)

sanitize: CFLAGS += $(DEBUG_FLAGS) $(SANITIZE_FLAGS)
sanitize: prepare $(BIN_DIR)/$(NAME)

run: debug
	@echo "Running $(NAME)..."
	@./$(BIN_DIR)/$(NAME)

test: debug
	@echo "Running tests..."
	@bash $(TEST_DIR)/test_commands.sh

clean:
	@echo "Cleaning build files..."
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

rebuild: clean debug

format:
	@echo "Formatting code..."
	@find $(SRC_DIR) -name "*.c" -o -name "*.h" | xargs clang-format -i

check-syntax:
	@echo "Checking syntax..."
	@$(CC) $(CFLAGS) -fsyntax-only $(SRCS)

valgrind: debug
	@echo "Running with valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all ./$(BIN_DIR)/$(NAME)

install: release
	@echo "Installing to /usr/local/bin..."
	@sudo cp $(BIN_DIR)/$(NAME) /usr/local/bin/

uninstall:
	@echo "Removing from /usr/local/bin..."
	@sudo rm -f /usr/local/bin/$(NAME)

help:
	@echo ""
	@echo "Available targets:"
	@echo ""
	@grep -E '^[a-zA-Z_-]+:.*?## ' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  %-12s %s\n", $$1, $$2}'
	@echo ""

prepare:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

$(BIN_DIR)/$(NAME): $(OBJS)
	@echo "Linking $@"
	@$(CC) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: debug release sanitize run test clean rebuild format check-syntax valgrind install uninstall help prepare
