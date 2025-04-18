#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define NUM_ITEMS 10  // Total items to produce

typedef struct {
    int buffer[BUFFER_SIZE];
    int front, rear, count;
    
    pthread_mutex_t lock;
    pthread_cond_t not_empty, not_full;
} bounded_buffer_t;

bounded_buffer_t buffer = { .front = 0, .rear = 0, .count = 0 };
int item_id = 0; // Global counter for unique items

void* producer(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < NUM_ITEMS / NUM_PRODUCERS; i++) {
        usleep(rand() % 1000000); // Random sleep to simulate work

        pthread_mutex_lock(&buffer.lock);

        // Wait if buffer is full
        while (buffer.count == BUFFER_SIZE) {
            pthread_cond_wait(&buffer.not_full, &buffer.lock);
        }

        // Produce an item
        int item = ++item_id;
        buffer.buffer[buffer.rear] = item;
        buffer.rear = (buffer.rear + 1) % BUFFER_SIZE;
        buffer.count++;

        printf("Producer %d produced: %d\n", id, item);

        // Signal consumers
        pthread_cond_signal(&buffer.not_empty);
        pthread_mutex_unlock(&buffer.lock);
    }
    return NULL;
}

void* consumer(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int i = 0; i < NUM_ITEMS / NUM_CONSUMERS; i++) {
        usleep(rand() % 1000000); // Random sleep to simulate work

        pthread_mutex_lock(&buffer.lock);

        // Wait if buffer is empty
        while (buffer.count == 0) {
            pthread_cond_wait(&buffer.not_empty, &buffer.lock);
        }

        // Consume an item
        int item = buffer.buffer[buffer.front];
        buffer.front = (buffer.front + 1) % BUFFER_SIZE;
        buffer.count--;

        printf("Consumer %d consumed: %d\n", id, item);

        // Signal producers
        pthread_cond_signal(&buffer.not_full);
        pthread_mutex_unlock(&buffer.lock);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_mutex_init(&buffer.lock, NULL);
    pthread_cond_init(&buffer.not_empty, NULL);
    pthread_cond_init(&buffer.not_full, NULL);

    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&producers[i], NULL, producer, id);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&consumers[i], NULL, consumer, id);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&buffer.lock);
    pthread_cond_destroy(&buffer.not_empty);
    pthread_cond_destroy(&buffer.not_full);

    printf("All threads completed.\n");
    return 0;
}

