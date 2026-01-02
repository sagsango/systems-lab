#include <stdio.h>

int main(void)
{
    printf("Hello from %s!\n", 
#ifdef __riscv
           "RISC-V"
#else
           "x86_64"
#endif
    );
    return 0;
}

