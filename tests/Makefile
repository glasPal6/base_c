CC := gcc
CFLAGS := -Wall -Wextra -std=c2x  # C23 standard (c2x is the flag for C23)
CFLAGS := $(CFLAGS) -I../base_c

# Add compiler-specific flags
ifeq ($(shell $(CC) -v 2>&1 | grep -c "clang version"), 1)
    # Using Clang
    CFLAGS += -fblocks -DCOMPILER_CLANG
else
    # Assuming GCC
    CFLAGS += -DCOMPILER_GCC
endif

LDFLAGS := 
SRC_DIR := src
BUILD_DIR := build

# Find all .c files in current directory
SRCS := $(wildcard $(SRC_DIR)/*.c)
# Generate object file names
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
# Generate test executable names
TESTS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%)

.PHONY: all clean

all: $(BUILD_DIR) $(TESTS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to build test executables
$(BUILD_DIR)/%: $(BUILD_DIR)/%.o
	$(CC) $(LDFLAGS) $< -o $@

# Rule to compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

# Run all tests
test: all
	@for test in $(TESTS) ; do \
		echo "Running $$test..." ; \
		$$test ; \
	done
