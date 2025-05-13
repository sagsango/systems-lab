#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define FILENAME "numbers.txt"

typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *head = NULL;
pthread_mutex_t lock;

void insert_node(int value) {
    Node *new_node = malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = head;
    head = new_node;
}

void *reader_thread(void *arg) {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("Failed to open file.\n");
        return NULL;
    }

    int value;
    while (fscanf(fp, "%d", &value) != EOF) {
        insert_node(value);  // Bug: shared memory, no lock
    }

    fclose(fp);
    return NULL;
}

void *summer_thread(void *arg) {
    int sum = 0;
    Node *cur = head;

    while (cur != NULL) {
        sum += cur->data;
        cur = cur->next;
    }

    printf("Sum = %d\n", sum);
    return NULL;
}

void *reverse_thread(void *arg) {
    Node *prev = NULL, *cur = head, *next = NULL;

    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    head = prev;
    return NULL;
}

void free_list() {
    Node *cur = head;
    while (cur) {
        Node *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
}

int main() {
    pthread_t t1, t2, t3;

    pthread_mutex_init(&lock, NULL);

    // Bug: file might not exist
    pthread_create(&t1, NULL, reader_thread, NULL);
    pthread_create(&t2, NULL, summer_thread, NULL);
    pthread_create(&t3, NULL, reverse_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    free_list();

    pthread_mutex_destroy(&lock);
    return 0;
}
