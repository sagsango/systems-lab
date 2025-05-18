#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#define REGION_SIZE 4096
#define FIXED_ADDR ((void *)0x5000000000)

int main() {
    // Step 1: Map some memory normally
    void *initial = mmap(FIXED_ADDR, REGION_SIZE,
                         PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
                         -1, 0);
    if (initial == MAP_FAILED) {
        perror("mmap MAP_FIXED");
        return 1;
    }

    printf("Mapped with MAP_FIXED at %p\n", initial);
    strcpy((char *)initial, "This was overwritten!");

    // Step 2: Try to map again at same address using MAP_FIXED_NOREPLACE
    void *second = mmap(FIXED_ADDR, REGION_SIZE,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE,
                        -1, 0);

    if (second == MAP_FAILED) {
        if (errno == EEXIST)
            printf("MAP_FIXED_NOREPLACE failed as expected: address already mapped\n");
        else
            perror("mmap MAP_FIXED_NOREPLACE");
    } else {
        printf("Unexpectedly mapped with MAP_FIXED_NOREPLACE at %p\n", second);
    }

    // Step 3: Map again with MAP_FIXED to overwrite the old mapping
    void *forced = mmap(FIXED_ADDR, REGION_SIZE,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
                        -1, 0);
    if (forced == MAP_FAILED) {
        perror("mmap overwrite with MAP_FIXED");
        return 1;
    }

    printf("Successfully remapped with MAP_FIXED at %p\n", forced);

    // Step 4: Check contents after remapping
    printf("After remapping, content = \"%s\"\n", (char *)forced);

    // Clean up
    munmap(forced, REGION_SIZE);
    return 0;
}

