#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handle_alarm(int sig) {
    if (sig == SIGALRM) {
        printf("Received SIGALRM signal. Alarm went off!\n");
        exit(0); // Exit after handling the signal
    }
}

int main() {
    if (signal(SIGALRM, handle_alarm) == SIG_ERR) {
        perror("signal");
        return 1;
    }

    printf("Setting alarm for 5 seconds...\n");
    alarm(5); // Set an alarm for 5 seconds

    printf("Waiting for alarm to go off...\n");
    while (1); /* Timer interrupt will take this procecc from userspace to kernel space, and when it wakes up then signal will be delevered */
    //pause();

    printf("This should not print.\n");

    return 0;
}

