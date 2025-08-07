.code16 # 16 bit mode = real mode; cpu starts from real mode.
    mov $0x2a, %ax    # Move 0x2a (42 decimal) into RAX
    hlt                # Halt the CPU
