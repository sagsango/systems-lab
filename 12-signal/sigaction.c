#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <ucontext.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#define PAGE_SIZE 4096
#define REGION_SIZE (PAGE_SIZE * 10)
#define BASE_ADDR ((void *)0x5000000000)

void *region_start;

void *page_align(void *addr) {
    uintptr_t a = (uintptr_t)addr;
    return (void *)(a & ~(PAGE_SIZE - 1));
}

void segv_handler(int sig, siginfo_t *si, void *unused) {
    void *fault_addr = si->si_addr;
    void *aligned_addr = page_align(fault_addr);

    fprintf(stderr, "[SIGSEGV] Fault at %p, aligning to %p\n", fault_addr, aligned_addr);

    if (fault_addr < region_start || fault_addr >= region_start + REGION_SIZE) {
        fprintf(stderr, "Address %p is outside handled range, exiting\n", fault_addr);
        exit(EXIT_FAILURE);
    }

    void *mapped = mmap(aligned_addr, PAGE_SIZE,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
                        -1, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap in signal handler");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "[SIGSEGV] Mapped page at %p\n", mapped);
}

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segv_handler;
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    region_start = mmap(BASE_ADDR, REGION_SIZE,
                        PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE | MAP_FIXED,
                        -1, 0);
    if (region_start == MAP_FAILED) {
        perror("mmap reserved region");
        exit(EXIT_FAILURE);
    }

    printf("[main] Reserved region at %p of size %lu bytes\n", region_start, REGION_SIZE);

    char *ptr = (char *)region_start + PAGE_SIZE * 2;
    printf("[main] Writing to address %p\n", ptr);
    *ptr = 'X';  // SIGSEGV

    printf("[main] Successfully wrote '%c' to %p\n", *ptr, ptr);
    return 0;




   // sigaction(int sig, const struct sigaction *restrict act, struct sigaction *restrict oact)
    // sigaltstack(const stack_t *restrict ss, stack_t *restrict oss)
    // sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oset)
    

}
