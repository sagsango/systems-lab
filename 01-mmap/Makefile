.phony: all clean run

all:mmap

mmap:mmap.c
	gcc -g3 -o mmap mmap.c

run:mmap
	./mmap mmap-out.txt
	xxd mmap-out.txt
clean:
	rm -rf mmap test.txt mmap-out.txt mmap.dSYM
