# Compiler and tools
CC      := gcc
RM      := rm -f
READ_ELF:= readelf
OBJDUMP := objdump
NM      := nm

# Common flags
CFLAGS  := -Wall -Wextra -Werror -g -O0 -fPIC
LDFLAGS_SO := -shared
LDFLAGS_DL := -ldl

# Files
LIB     := libmylib.so
LIBSRC  := mylib.c
LIBOBJ  := mylib.o

MAIN    := main
MAINSRC := main.c
MAINOBJ := main.o

DLMAIN    := dlmain
DLMAINSRC := dlmain.c
DLMAINOBJ := dlmain.o

.PHONY: all clean run run-dlopen info sections initarray symbols disasm help relink

all: $(LIB) $(MAIN) $(DLMAIN)

# Build shared library
$(LIB): $(LIBOBJ)
	$(CC) $(LDFLAGS_SO) -o $@ $^

$(LIBOBJ): $(LIBSRC) mylib.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Build main linked against the shared library
$(MAIN): $(MAINOBJ) $(LIB)
	$(CC) -g -O0 -o $@ $(MAINOBJ) -L. -lmylib

$(MAINOBJ): $(MAINSRC) mylib.h
	$(CC) -Wall -Wextra -Werror -g -O0 -c -o $@ $<

# Build dlopen test program
$(DLMAIN): $(DLMAINOBJ)
	$(CC) -g -O0 -o $@ $(DLMAINOBJ) $(LDFLAGS_DL)

$(DLMAINOBJ): $(DLMAINSRC)
	$(CC) -Wall -Wextra -Werror -g -O0 -c -o $@ $<

# Run normal dynamically linked executable
run: $(LIB) $(MAIN)
	LD_LIBRARY_PATH=. ./$(MAIN)

# Run dlopen() based executable
run-dlopen: $(LIB) $(DLMAIN)
	./$(DLMAIN)

# Show useful ELF metadata
info: $(LIB)
	@echo "===== file type ====="
	file $(LIB)
	@echo
	@echo "===== dynamic section ====="
	$(READ_ELF) -d $(LIB)
	@echo
	@echo "===== program headers ====="
	$(READ_ELF) -l $(LIB)

# Show sections including .init_array / .fini_array
sections: $(LIB)
	@echo "===== section headers ====="
	$(READ_ELF) -S $(LIB)
	@echo
	@echo "===== .init_array hex dump ====="
	$(READ_ELF) -x .init_array $(LIB) || true
	@echo
	@echo "===== .fini_array hex dump ====="
	$(READ_ELF) -x .fini_array $(LIB) || true

# More focused target for constructor/destructor arrays
initarray: $(LIB)
	@echo "===== symbols related to init/fini ====="
	$(NM) -an $(LIB) | egrep 'lib_init|lib_fini|init_array|fini_array|_init|_fini' || true
	@echo
	@echo "===== relocation info ====="
	$(READ_ELF) -r $(LIB)
	@echo
	@echo "===== .init_array dump ====="
	$(READ_ELF) -x .init_array $(LIB) || true
	@echo
	@echo "===== disassembly near lib_init ====="
	$(OBJDUMP) -drwC $(LIB) | sed -n '/<lib_init>/,/<.*>:/p'

# Symbol table
symbols: $(LIB)
	@echo "===== dynamic symbols ====="
	$(NM) -D $(LIB)
	@echo
	@echo "===== all symbols ====="
	$(NM) -an $(LIB)

# Full disassembly
disasm: $(LIB)
	$(OBJDUMP) -drwC $(LIB)

# Force rebuild experiment
relink: clean all

clean:
	$(RM) $(LIBOBJ) $(MAINOBJ) $(DLMAINOBJ)
	$(RM) $(LIB) $(MAIN) $(DLMAIN)
