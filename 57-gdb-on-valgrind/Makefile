.phony: all clean

EXE=a

all:$(EXE)

a:a.c
	gcc -g3 -o a a.c

run: a
	valgrind ./a

clean:
	rm -rf *.o $(EXE)

debug:
	@echo "run this on another session"
	@echo "$ gdb -x gdb.init"
	@echo "# gdb will stop when valgrind detect error;"
	@echo "# use continue to go to next error"
	@echo "# If dont work use: <see the messge from 1st shell"
	@echo "# target remote | /usr/bin/vgdb --pid=23110 "
	valgrind --vgdb=yes --vgdb-error=0 ./a
