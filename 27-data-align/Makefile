
all: 01-O0 01-O1 01-O2 01-O3

01-O0: 01.c
	gcc -O0 -fopt-info-all=opt-O0.txt -o 01-O0 01.c -lpthread

01-O1: 01.c
	gcc -O1 -fopt-info-all=opt-O1.txt -o 01-O1 01.c -lpthread

01-O2: 01.c
	gcc -O2 -fopt-info-all=opt-O2.txt -o 01-O2 01.c -lpthread

01-O3: 01.c
	gcc -O3 -fopt-info-all=opt-O3.txt -o 01-O3 01.c -lpthread

clean:
	rm -f 01-O0 01-O1 01-O2 01-O3 opt-O*.txt

