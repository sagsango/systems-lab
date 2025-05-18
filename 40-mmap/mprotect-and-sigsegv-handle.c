#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define PAGE_SIZE 4096

char *page;

void segv_handler(int sig, siginfo_t *si, void *ctx) {
    void *addr = si->si_addr;

    uintptr_t aligned = (uintptr_t)addr & ~(PAGE_SIZE - 1);

    fprintf(stderr, "[SIGSEGV] Caught at %p, unprotecting page %p\n", addr, (void *)aligned);

    if (mprotect((void *)aligned, PAGE_SIZE, PROT_WRITE) == -1) {
        perror("mprotect PROT_WRITE in handler");
        exit(EXIT_FAILURE);
    }
}

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segv_handler;
    if (sigaction(SIGSEGV, &sa, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    page = mmap(NULL, PAGE_SIZE, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (page == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (mprotect(page, PAGE_SIZE, PROT_NONE) == -1) {
        perror("mprotect PROT_NONE");
        exit(EXIT_FAILURE);
    }

    printf("Writing one character at a time with page fault for each write:\n");

    for (int i = 0; i < 26; i++) {
        char c = 'A' + i;
        page[i] = c;  // SIGSEGV

        if (mprotect(page, PAGE_SIZE, PROT_NONE) == -1) {
            perror("mprotect PROT_NONE after write");
            exit(EXIT_FAILURE);
        }

        printf("Wrote '%c' at page[%d]\n", c, i);
    }

    printf("Final content: %.26s\n", page);

    return 0;
}
