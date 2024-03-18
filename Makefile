CC := gcc
CFLAGS := -std=c89 -ansi -pedantic -Wall -Wpedantic

# Check if the DEBUG variable is set
ifdef DEBUG
    CFLAGS += -g
endif

SRCDIR := src
OBJDIR := obj
BINDIR := bin

# Use wildcard to automatically find all .c files in the source directory and its subdirectories
SOURCES := $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/*/*.c)

# Generate a list of object files
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Output binary
TARGET := $(BINDIR)/assembler

# Default target
all: $(TARGET)

# Rule to make the target
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Rule to make object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	@rm -rf $(OBJDIR) $(BINDIR)

# Phony target to avoid conflicts with file names
.PHONY: all clean
