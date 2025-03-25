.phony: clean

all: small_test big_test random_test

small_test:small_test.c allocator
	gcc -g3 -o small_test small_test.c allocator.o

big_test:big_test.c allocator
	gcc -g3 -o big_test big_test.c allocator.o

random_test:random_test.c allocator
	gcc -g3 -o random_test random_test.c allocator.o

allocator: allocator.c
	gcc -g3 -c  allocator.c -o allocator.o

clean:
	rm  allocator.o small_test big_test random_test
	rm -rf *.dSYM
