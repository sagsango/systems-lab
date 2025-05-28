# Compiler and flags
CC = gcc
CFLAGS = -I./include
OBJDIR = obj
LIBDIR = lib
SRCDIR = src
INCLUDEDIR = include

# Object files
OBJ = $(OBJDIR)/mymath.o
EXE = exe_with_static_lib exe_with_dynamic_lib statically_linked proc_map

# Default target
all: $(EXE) run

# Create directories
$(shell mkdir -p $(OBJDIR) $(LIBDIR))

# Compile source to object file with PIC
$(OBJDIR)/mymath.o: $(SRCDIR)/mymath.c $(INCLUDEDIR)/mymath.h
	$(CC) $(CFLAGS) -c -fPIC $(SRCDIR)/mymath.c -o $(OBJDIR)/mymath.o

# Static library
$(LIBDIR)/libmymath.a: $(OBJDIR)/mymath.o
	ar rcs $(LIBDIR)/libmymath.a $(OBJDIR)/mymath.o

# Dynamic library
$(LIBDIR)/libmymath.so: $(OBJDIR)/mymath.o
	$(CC) -shared -o $(LIBDIR)/libmymath.so $(OBJDIR)/mymath.o

# Test program with static library
exe_with_static_lib: $(LIBDIR)/libmymath.a main.c
	$(CC) $(CFLAGS) main.c $(LIBDIR)/libmymath.a -o exe_with_static_lib

# Test program with dynamic library
exe_with_dynamic_lib: $(LIBDIR)/libmymath.so main.c
	$(CC) $(CFLAGS) main.c -L./lib -lmymath -o exe_with_dynamic_lib

# Test program which is statically linked (uses static libaries, and ignores .so)
statically_linked:$(LIBDIR) main.c
	$(CC) -v $(CFLAGS) -static main.c -L./lib -lmymath -o statically_linked

proc_map: proc_map.c
	$(CC) -o proc_map  proc_map.c

# Run tests
run: $(EXC)
	@echo "===================================================="
	@echo "<0> Lets see all the lib present (.a = static, .so = dynamic)"
	@echo "===================================================="
	ls -l lib/
	@echo "===================================================="
	@echo "<1> Lets see the exe with static lib and dynamic lib"
	@echo "===================================================="
	file exe_with_static_lib
	file exe_with_dynamic_lib
	ldd exe_with_static_lib
	ldd exe_with_dynamic_lib
	@echo "===================================================="
	@echo "<2> static library code get added during linked time (more disk space)"
	@echo "===================================================="
	ls -l exe_with_static_lib
	ls -l exe_with_dynamic_lib
	@echo "===================================================="
	@echo "<2.1> static library code got added in statically linked exe" 
	@echo "===================================================="
	objdump -D exe_with_static_lib > exe_with_static_lib.objdump.out
	objdump -D exe_with_dynamic_lib > exe_with_dynamic_lib.objdump.out
	grep subtract exe_with_static_lib.objdump.out
	grep subtract exe_with_dynamic_lib.objdump.out
	@echo "===================================================="
	@echo "<3> Program which are statically linked, all the libraries are resolved linked time"
	@echo "    Exe will include all the libraries"
	@echo "    It mean that all the .so will be ignored by the linker"
	@echo "    Linker will use all the .a only"
	@echo "===================================================="
	ldd statically_linked || true
	ls -l statically_linked
	@echo "===================================================="
	@echo "<4> Which part of the shared libary are shared?"
	@echo "    libc's : .rodata & .text are shared (same pf)"
	@echo "           : r--p & r-xp"
	@echo "    libc's : .data & .bss are private (diff pf per process"
	@echo "           : rw-p & ---p, (---p = no pf allocated just guard page)"
	@echo "    As some parts of the files are shared and some are private"
	@echo "            whole file is privatly mmaped"
	@echo "===================================================="
	./proc_map
	@echo "===================================================="
	@echo "TODO: "
	@echo "===================================================="
	@echo "	1.	kernel code which involve the mapping of shared librarys"
	@echo "	2.	ld code and elf-loading and where all the shared lib are stored"
	@echo "	3.	userspace loader in compound is not saving the memory? by sharing libs"

# Clean up
clean:
	rm -rf $(OBJDIR) $(LIBDIR) $(EXE) *.out

.PHONY: all static dynamic clean
