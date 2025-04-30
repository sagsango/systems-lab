#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <errno.h>

#define PAGE_SIZE 4096
#define NUM_PAGES 1

void *region_start;

static void segv_handler(int sig, siginfo_t *si, void *unused) {
    uintptr_t fault_addr = (uintptr_t)si->si_addr;
    uintptr_t base = (uintptr_t)region_start;

    // Calculate the page-aligned address
    uintptr_t page_start = fault_addr & ~(PAGE_SIZE - 1);
    size_t offset = fault_addr & (PAGE_SIZE -1);

    printf("SIGSEGV at address %p (page offset = %zu)\n",
	   si->si_addr, offset);

    // Fill the page with the page offset value
    mprotect((void *)page_start, PAGE_SIZE, PROT_WRITE);

    char *page_data = (char *)page_start;
    *(long *)(page_data + offset) = offset;


    mprotect((void *)page_start, PAGE_SIZE, PROT_READ); // Allow future reads
}

int main() {
    // Install SIGSEGV handler
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = segv_handler;
    sigemptyset(&sa.sa_mask);
    assert(sigaction(SIGSEGV, &sa, NULL) == 0);

    // mmap region
    region_start = mmap(NULL, NUM_PAGES * PAGE_SIZE, PROT_READ | PROT_WRITE,
                        MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (region_start == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Protect all pages
    mprotect(region_start, NUM_PAGES * PAGE_SIZE, PROT_NONE);

    // Trigger read (will cause SIGSEGV and lazy fill)
    for (int i = 0; i + 64 < PAGE_SIZE; i+=64) {
        long *ptr = (long *)((char*)region_start + i);
        printf("Read from page %d: %ld\n", i, ptr[0]); // Should print page index
	mprotect((void *)region_start, PAGE_SIZE, PROT_NONE); // Allow future reads
    }

    // Clean up
    munmap(region_start, NUM_PAGES * PAGE_SIZE);
    return 0;
}

