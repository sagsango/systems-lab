// TODO:
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/mman.h>
#include <unistd.h>

#define REGION_SIZE 4096
void *region;

void handler(int sig) {
    printf("SIGSEGV received, but can't get faulting address with signal()\n");
    exit(1);
}

int main() {
    signal(SIGSEGV, handler);

    region = mmap(NULL, REGION_SIZE, PROT_NONE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    char *ptr = (char *)region + 100;
    *ptr = 'A'; // SIGSEGV

    return 0;
}
