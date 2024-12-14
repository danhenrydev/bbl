# Compiler and flags
CC := clang
CFLAGS := -Wall -Wextra -g -xc -std=c99 -I./include
LDFLAGS :=

EXEC_NAME := bbc

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Find all source files recursively
SRCS := $(shell find $(SRC_DIR) -name '*.c')
# Generate object file paths by replacing src/ with obj/ and .c with .o
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# Final executable name
EXEC := $(BIN_DIR)/$(EXEC_NAME)

# First rule is the default rule
.PHONY: all
all: $(EXEC)

# Rule to create directories if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Rule to create object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the executable
$(EXEC): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $@

# Clean rule to remove generated files
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Debug rule to print variables
.PHONY: debug
debug:
	@echo "Source files:" $(SRCS)
	@echo "Object files:" $(OBJS)

.PHONY: compiledb
bear: clean
	bear -- make
