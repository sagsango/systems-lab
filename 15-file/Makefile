# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Find all .c files in the current directory
SOURCES = $(wildcard *.c)

# Generate executable names (replace .c with no extension)
EXECS = $(SOURCES:.c=)

# Default target
all: open-open_dir #$(EXEC)

# Rule to link object files into executables
%: %.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up
clean:
	rm -f $(EXECS)

# Phony targets
.PHONY: all clean
