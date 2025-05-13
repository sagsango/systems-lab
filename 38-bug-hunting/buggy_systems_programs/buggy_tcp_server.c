#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    char buffer[1024];
    read(client_fd, buffer, sizeof(buffer)); // 🔴 No size check
    printf("Client: %s\n", buffer);
    close(client_fd); // 🔴 May leak on failure
    return NULL;
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    listen(server_fd, 5);

    while (1) {
        int client_fd = accept(server_fd, NULL, NULL);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, &client_fd); // 🔴 Shared arg
        pthread_detach(tid);
    }

    return 0;
}