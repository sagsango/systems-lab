.phony: all clean run

all:mmap01 mmap02 mmap03

mmap01:mmap01.c
	gcc -o mmap01 mmap01.c

mmap02:mmap02.c
	gcc -g3 -o mmap02 mmap02.c

mmap02:mmap03.c
	gcc -g3 -o mmap03 mmap03.c

run01:mmap01
	./mmap01 mmap01.txt
	xxd mmap01.txt

run02:mmap02
	./mmap02 mmap02.txt
	xxd mmap02.txt

run03:mmap03
	./mmap03 mmap03.txt
	xxd mmap03.txt



clean:
	rm -rf mmap01 mmap01.txt mmap02 mmap02.txt mmap03 mmap03.txt
