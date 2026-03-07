#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void sigtrap_handler(int signo) {
    printf("Caught signal %d (SIGTRAP)\n", signo);
    fflush(stdout);
}

int main(void) {
    struct sigaction sa;

    sa.sa_handler = sigtrap_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGTRAP, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    printf("Before int3\n");
    fflush(stdout);

    __asm__ volatile("int3");

    printf("After int3\n");
    return 0;
}
