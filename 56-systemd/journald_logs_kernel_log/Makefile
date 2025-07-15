CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lsystemd
TARGET = dummy-journald
SOURCES = dummy-journald.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	./deploy.sh

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
