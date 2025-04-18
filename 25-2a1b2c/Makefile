all: main

main:main.c
	gcc -o main main.c

run:main
	./main > out  2>&1

check:check.c
	gcc -o check check.c

test:run check
	./check
	
clean:
	rm main out check
