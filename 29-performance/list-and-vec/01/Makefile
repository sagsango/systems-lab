CC = gcc
CFLAGS = -Wall -Wextra
OPT_FLAGS = -O0 -O1 -O2 -O3
VECTOR_FLAGS = -O3 -march=native
SRC = list-vs-vector.c
EXECS = compare_O0 compare_O1 compare_O2 compare_O3 compare_vector

# Default target: build all executables
all: $(EXECS)

# Rule for -O0 (no optimization)
compare_O0: $(SRC)
	$(CC) $(CFLAGS) -O0 -o compare_O0 $(SRC)

# Rule for -O1
compare_O1: $(SRC)
	$(CC) $(CFLAGS) -O1 -o compare_O1 $(SRC)

# Rule for -O2
compare_O2: $(SRC)
	$(CC) $(CFLAGS) -O2 -o compare_O2 $(SRC)

# Rule for -O3
compare_O3: $(SRC)
	$(CC) $(CFLAGS) -O3 -o compare_O3 $(SRC)

# Rule for -O3 with vectorization (-march=native)
compare_vector: $(SRC)
	$(CC) $(CFLAGS) -O3 -march=native -o compare_vector $(SRC)

# Target to run all executables
run: all
	@echo "Running compare_O0..."
	./compare_O0
	@echo "\nRunning compare_O1..."
	./compare_O1
	@echo "\nRunning compare_O2..."
	./compare_O2
	@echo "\nRunning compare_O3..."
	./compare_O3
	@echo "\nRunning compare_vector..."
	./compare_vector

# Clean up generated files
clean:
	rm -f $(EXECS)

# Phony targets
.PHONY: all run clean
