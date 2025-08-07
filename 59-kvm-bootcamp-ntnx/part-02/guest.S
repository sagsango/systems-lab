.code16 # 16 bit mode = real mode; cpu starts from real mode.
    mov $0x10, %ax    # Move 0x2a (42 decimal) into RAX
    mov $0x60, %dx
    out %ax, %dx
    hlt                # Halt the CPU
