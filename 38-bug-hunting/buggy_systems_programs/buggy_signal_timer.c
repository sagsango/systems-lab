#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int count = 0;

void handler(int sig) {
    printf("Timer %d\n", ++count); // 🔴 Unsafe in signal handler
}

int main() {
    signal(SIGALRM, handler);
    alarm(1);
    while (count < 5) pause();
    return 0;
}