CC := gcc
CFLAGS := -std=c11 -ansi -pedantic -Wall -Wno-comment -Wunused-variable

# Check if the DEBUG variable is set
ifdef DEBUG
    CFLAGS += -g
endif

SRCDIR := src
OBJDIR := obj
BINDIR := bin

# Use wildcard to automatically find all .c files in the source directory
SOURCES := $(wildcard $(SRCDIR)/*.c)

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
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	@rm -rf $(OBJDIR) $(BINDIR)

# Phony target to avoid conflicts with file names
.PHONY: all clean
