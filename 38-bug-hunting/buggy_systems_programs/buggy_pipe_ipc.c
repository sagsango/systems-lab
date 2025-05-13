#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    pipe(fd); // 🔴 No error check

    pid_t pid = fork();

    if (pid == 0) {
        close(fd[1]);
        char buffer[100];
        read(fd[0], buffer, 100); // 🔴 Partial read unhandled
        printf("Child received: %s\n", buffer);
        exit(0);
    } else {
        close(fd[0]);
        char *msg = "Hello from parent!";
        write(fd[1], msg, strlen(msg)); // 🔴 No null terminator
        wait(NULL);
    }

    return 0;
}