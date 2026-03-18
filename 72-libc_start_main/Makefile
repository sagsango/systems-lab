all:
	gcc -Wall -fPIC -shared -g -o mypreload.so mypreload.c -ldl
	gcc -O0 -g -o main main.c
	LD_PRELOAD=./mypreload.so ./main

clean:
	rm ./mypreload.so ./main

