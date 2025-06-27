// sum_no_glibc.c
__attribute__((naked)) void _start() {
    // Compute sum
    int u = 1;
    int i = 2;
    int c = 3;
    int result = u+i+c;

    while(1);

    // Use the Linux 'exit' syscall directly to return result as exit code
    __asm__ volatile (
        "movl $60, %%eax\n"     // syscall number for exit (x86_64)
        "movl %0, %%edi\n"      // exit code = result
        "syscall\n"
        :
        : "r"(result)
        : "%eax", "%edi"
    );
}
