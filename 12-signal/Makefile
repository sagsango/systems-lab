SOURCES := $(wildcard *.c)

TARGETS := $(basename $(SOURCES))

all: $(TARGETS)

%: %.c
	$(CC) -Wall -Wextra -O2 -o $@ $<

clean:
	rm -f $(TARGETS)
