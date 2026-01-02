# =========================================
# Multi-arch build + QEMU linux-user runner
# =========================================

SRC := hello.c

X86_BIN   := hello-x86
RISCV_BIN := hello-riscv

CC_X86    := gcc
CC_RISCV  := riscv64-linux-gnu-gcc

CFLAGS    := -O0 -g

.PHONY: all info prompt build run clean

# Default target
all:build

# -----------------------------------------
# Prompt user for QEMU build directory
# -----------------------------------------
prompt:
	@read -p "Enter QEMU build directory (containing qemu-riscv64, qemu-x86_64): " QEMU_BUILD_DIR; \
	if [ ! -x "$$QEMU_BUILD_DIR/qemu-riscv64" ]; then \
		echo "ERROR: $$QEMU_BUILD_DIR/qemu-riscv64 not found or not executable"; \
		exit 1; \
	fi; \
	if [ ! -x "$$QEMU_BUILD_DIR/qemu-x86_64" ]; then \
		echo "ERROR: $$QEMU_BUILD_DIR/qemu-x86_64 not found or not executable"; \
		exit 1; \
	fi; \
	echo "$$QEMU_BUILD_DIR" > .qemu_build_dir

# -----------------------------------------
# Info
# -----------------------------------------
info:
	@QEMU_BUILD_DIR=$$(cat .qemu_build_dir); \
	echo "========================================"; \
	echo "Using QEMU linux-user from:"; \
	echo "  $$QEMU_BUILD_DIR"; \
	echo "========================================"; \
	echo " - x86_64  -> $$QEMU_BUILD_DIR/qemu-x86_64"; \
	echo " - riscv64 -> $$QEMU_BUILD_DIR/qemu-riscv64"; \
	echo "========================================"

# -----------------------------------------
# Build
# -----------------------------------------
build: $(X86_BIN) $(RISCV_BIN)

$(X86_BIN): $(SRC)
	@echo "[BUILD] x86_64 binary"
	$(CC_X86) $(CFLAGS) $< -o $@

$(RISCV_BIN): $(SRC)
	@echo "[BUILD] riscv64 binary"
	$(CC_RISCV) $(CFLAGS) -static $< -o $@

# -----------------------------------------
# Run
# -----------------------------------------
run:prompt info
	@QEMU_BUILD_DIR=$$(cat .qemu_build_dir); \
	echo "----------------------------------------"; \
	echo "[RUN] x86_64 via QEMU linux-user"; \
	echo "----------------------------------------"; \
	$$QEMU_BUILD_DIR/qemu-x86_64 ./$(X86_BIN); \
	echo "----------------------------------------"; \
	echo "[RUN] riscv64 via QEMU linux-user"; \
	echo "----------------------------------------"; \
	$$QEMU_BUILD_DIR/qemu-riscv64 ./$(RISCV_BIN)

objdump:
	riscv64-linux-gnu-objdump -d hello-riscv > objdump-riscv.out
	x86_64-linux-gnu-objdump -d hello-x86 > objdump-x86_64.out
# -----------------------------------------
# Clean
# -----------------------------------------
clean:
	rm -f $(X86_BIN) $(RISCV_BIN) .qemu_build_dir
	rm -f objdump-riscv.out objdump-x86_64.out

