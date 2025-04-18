#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_THREADS 4
#define QUEUE_SIZE 10

typedef struct {
    void (*function)(void*); // Function pointer
    void* argument;          // Function argument
} task_t;

typedef struct {
    task_t task_queue[QUEUE_SIZE];  // Task queue
    int front, rear, count;         // Queue control variables

    pthread_t threads[MAX_THREADS]; // Worker threads
    pthread_mutex_t lock;           // Mutex lock
    pthread_cond_t notify;          // Condition variable
    
    bool shutdown;                  // Shutdown flag
} thread_pool_t;

// Function prototypes
void* worker_thread(void* arg);
void thread_pool_init(thread_pool_t* pool);
void thread_pool_submit(thread_pool_t* pool, void (*function)(void*), void* arg);
void thread_pool_destroy(thread_pool_t* pool);

// Initialize thread pool
void thread_pool_init(thread_pool_t* pool) {
    pool->front = pool->rear = pool->count = 0;
    pool->shutdown = false;
    
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->notify, NULL);

    // Create worker threads
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&pool->threads[i], NULL, worker_thread, (void*) pool);
    }
}

// Task submission
void thread_pool_submit(thread_pool_t* pool, void (*function)(void*), void* arg) {
    pthread_mutex_lock(&pool->lock);
    
    if (pool->count == QUEUE_SIZE) {
        printf("Queue is full! Task rejected.\n");
        pthread_mutex_unlock(&pool->lock);
        return;
    }

    // Add task to queue
    pool->task_queue[pool->rear].function = function;
    pool->task_queue[pool->rear].argument = arg;
    pool->rear = (pool->rear + 1) % QUEUE_SIZE;
    pool->count++;

    // Signal worker threads
    pthread_cond_signal(&pool->notify);
    pthread_mutex_unlock(&pool->lock);
}

// Worker thread function
void* worker_thread(void* arg) {
    thread_pool_t* pool = (thread_pool_t*) arg;

    while (1) {
        pthread_mutex_lock(&pool->lock);

        // Wait for a task
        while (pool->count == 0 && !pool->shutdown) {
            pthread_cond_wait(&pool->notify, &pool->lock);
        }

        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }

        // Fetch task from queue
        task_t task = pool->task_queue[pool->front];
        pool->front = (pool->front + 1) % QUEUE_SIZE;
        pool->count--;

        pthread_mutex_unlock(&pool->lock);

        // Execute task
        task.function(task.argument);
    }
}

// Destroy thread pool
void thread_pool_destroy(thread_pool_t* pool) {
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = true;
    pthread_cond_broadcast(&pool->notify);
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->notify);
}

// Example task function
void example_task(void* arg) {
    int num = *(int*)arg;
    printf("Processing task: %d\n", num);
    sleep(1);
}

int main() {
    thread_pool_t pool;
    thread_pool_init(&pool);

    // Submit tasks
    for (int i = 0; i < 10; i++) {
        int* task_num = malloc(sizeof(int));
        *task_num = i;
        thread_pool_submit(&pool, example_task, task_num);
    }

    sleep(5); // Wait for tasks to finish
    thread_pool_destroy(&pool);
    return 0;
}

