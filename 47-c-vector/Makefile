# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -fPIC -g3
LDFLAGS = -shared
RPATH = -Wl,-rpath=.

# Libraries
LIBVECTOR = libvector.so
LIBVECTOR_INT = libvector_int.so

# Source files
VECTOR_SRCS = vector.c vec_internal.c
VECTOR_OBJS = $(VECTOR_SRCS:.c=.o)

VECTOR_INT_SRCS = vector_int.c
VECTOR_INT_OBJS = $(VECTOR_INT_SRCS:.c=.o)

# Test files
TESTS = test_vec_internal test_vector test_vector_int

TEST_VEC_INTERNAL_SRC = test_vec_internal.c
TEST_VECTOR_SRC = test_vector.c
TEST_VECTOR_INT_SRC = test_vector_int.c

TEST_VEC_INTERNAL_OBJ = $(TEST_VEC_INTERNAL_SRC:.c=.o)
TEST_VECTOR_OBJ = $(TEST_VECTOR_SRC:.c=.o)
TEST_VECTOR_INT_OBJ = $(TEST_VECTOR_INT_SRC:.c=.o)

.PHONY: all clean

# Default target
all: $(LIBVECTOR) $(LIBVECTOR_INT) $(TESTS)

# Build shared libraries
$(LIBVECTOR): $(VECTOR_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(LIBVECTOR_INT): $(VECTOR_INT_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -L. -lvector -Wl,-rpath=.

# Build test executables
test_vec_internal: $(TEST_VEC_INTERNAL_OBJ) $(LIBVECTOR)
	$(CC) -o $@ $^ -L. -lvector $(RPATH)

test_vector: $(TEST_VECTOR_OBJ) $(LIBVECTOR)
	$(CC) -o $@ $^ -L. -lvector $(RPATH)

test_vector_int: $(TEST_VECTOR_INT_OBJ) $(LIBVECTOR_INT)
	$(CC) -o $@ $^ -L. -lvector_int $(RPATH)

# Compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean all
clean:
	rm -f *.o *.so $(TESTS)

