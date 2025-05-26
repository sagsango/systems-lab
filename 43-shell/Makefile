# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Find all .c files in the current directory
SOURCES = $(wildcard *.c)

# Generate executable names (replace .c with no extension)
EXECS = $(SOURCES:.c=)

# Default target
all: $(EXECS)

# Rule to link object files into executables
%: %.c
	$(CC) $(CFLAGS) $< -o $@


trace: all
	ltrace -S -n 20 -o ltrace.out ./piped-cmds
	strace -f --stack-traces -s 128 -o strace.out ./piped-cmds

# Clean up
clean:
	rm -f $(EXECS)

# Phony targets
.PHONY: all clean
