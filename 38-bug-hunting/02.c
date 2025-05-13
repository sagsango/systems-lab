#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MAX_TASKS 100
#define LOG_FILE "log.txt"

typedef struct Task {
    int id;
    char *description;
    struct Task *next;
} Task;

typedef struct {
    Task *head;
    Task *tail;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} TaskQueue;

TaskQueue queue;
FILE *log_fp;

void enqueue(Task *task) {
    // 🔴 Missing lock
    if (queue.tail) {
        queue.tail->next = task;
        queue.tail = task;
    } else {
        queue.head = queue.tail = task;
    }
    queue.count++;
    pthread_cond_signal(&queue.cond); // 🔴 Should be under lock
}

Task* dequeue() {
    pthread_mutex_lock(&queue.lock);
    while (queue.head == NULL) {
        pthread_cond_wait(&queue.cond, &queue.lock);
    }

    Task *task = queue.head;
    queue.head = queue.head->next;
    if (queue.head == NULL) queue.tail = NULL;
    queue.count--;
    pthread_mutex_unlock(&queue.lock);
    return task;
}

void *producer(void *arg) {
    for (int i = 0; i < MAX_TASKS; i++) {
        Task *task = malloc(sizeof(Task));
        task->id = i;
        task->description = malloc(30);
        sprintf(task->description, "Task #%d", i);
        task->next = NULL;

        enqueue(task);

        usleep(rand() % 10000);
    }
    return NULL;
}

void log_task(Task *task) {
    fprintf(log_fp, "Processing %s\n", task->description);  // 🔴 Not thread-safe
    fflush(log_fp);
}

void *consumer(void *arg) {
    while (1) {
        Task *task = dequeue();
        log_task(task);

        // Simulate work
        usleep(rand() % 5000);

        free(task->description);  // 🔴 May be freed twice
        free(task);
    }
    return NULL;
}

int main() {
    pthread_t prod_thread;
    pthread_t cons_threads[4];

    queue.head = queue.tail = NULL;
    queue.count = 0;
    pthread_mutex_init(&queue.lock, NULL);
    pthread_cond_init(&queue.cond, NULL);

    log_fp = fopen(LOG_FILE, "w");  // 🔴 Never checked for NULL

    pthread_create(&prod_thread, NULL, producer, NULL);
    for (int i = 0; i < 4; i++) {
        pthread_create(&cons_threads[i], NULL, consumer, NULL);
    }

    pthread_join(prod_thread, NULL);

    sleep(1);  // 🔴 Hope consumers finish? Not robust
    fclose(log_fp);  // 🔴 Other threads may still write

    pthread_mutex_destroy(&queue.lock);
    pthread_cond_destroy(&queue.cond);

    return 0;
}
