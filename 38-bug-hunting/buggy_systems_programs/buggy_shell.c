#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_CMD 1024

void handle_sigint(int sig) {
    printf("Received SIGINT\n"); // 🔴 Not async-signal-safe
}

int main() {
    char cmd[MAX_CMD];

    signal(SIGINT, handle_sigint); // 🔴 Unsafe in production code

    while (1) {
        printf("bugsh> ");
        fgets(cmd, MAX_CMD, stdin); // 🔴 No error check

        if (strncmp(cmd, "exit", 4) == 0) break;

        cmd[strcspn(cmd, "\n")] = 0; // Remove newline
        char *args[100];
        int i = 0;
        args[i] = strtok(cmd, " ");
        while (args[i] != NULL) args[++i] = strtok(NULL, " ");

        pid_t pid = fork();
        if (pid == 0) {
            execvp(args[0], args); // 🔴 No error check
            printf("Failed to execute\n");
            exit(1);
        } else if (pid > 0) {
            // 🔴 Doesn't reap background processes
            waitpid(pid, NULL, 0);
        } else {
            printf("Fork failed\n");
        }
    }
    return 0;
}