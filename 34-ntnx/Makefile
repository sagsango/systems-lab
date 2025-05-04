CC = gcc

# Our library supports link time optimizations -flto
CFLAGS = -Wall -Wextra -fPIC  -g -pthread -flto
LDFLAGS = -shared
LIBNAME = libntnx_hash.so
TARGET = test_ntnx_hash

.PHONY: all clean test

all: $(LIBNAME) $(TARGET)

# build shared library
$(LIBNAME): ntnx_hash.o
	$(CC) $(LDFLAGS) -o $@ $^

# build test executable
$(TARGET): test_ntnx_hash.o
	$(CC) -Wall -g -pthread -o $@ $^ -L. -lntnx_hash

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# run the tests
test: $(TARGET)
	LD_LIBRARY_PATH=. ./$(TARGET)

clean:
	rm -f *.o $(TARGET) $(LIBNAME)

