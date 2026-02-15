# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -O3 -g -lm
TARGET := mylang

SRCS := $(shell find . -name '*.c')

# Object directory and object files
OBJDIR := build
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

# Default rule
.PHONY: all clean
all: $(TARGET)

# Link all object files into final binary
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c to .o
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf $(OBJDIR) $(TARGET)
