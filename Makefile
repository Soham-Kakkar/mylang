# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O2 -g -lm
TARGET := mylang

# Find all .c and .h files in all subdirectories
SRCS := $(shell find . -name '*.c')

# Object directory and object files
OBJDIR := build
OBJS := $(patsubst ./%, $(OBJDIR)/%, $(SRCS:.c=.o))

# Default rule
.PHONY: all clean
all: $(TARGET)

# Link all object files into final binary
$(TARGET): $(OBJS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c to .o, creating directory structure in build/
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJDIR) $(TARGET)
