#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_THREADS 4  // Number of threads in the pool
#define MAX_TASKS 10   // Maximum number of tasks in the queue

// Define a task structure
typedef struct task {
    void (*function)(void *);  // Function pointer to the task
    void *arg;                 // Argument to be passed to the function
} task_t;

// Define the thread pool structure
typedef struct thread_pool {
    pthread_t *threads;         // Array of threads
    pthread_mutex_t lock;       // Mutex for task queue
    pthread_cond_t cond;        // Condition variable for task arrival
    task_t task_queue[MAX_TASKS]; // Queue of tasks
    int task_count;             // Current number of tasks in the queue
    int stop;                   // Flag to indicate shutdown
} thread_pool_t;

// Function to initialize the thread pool
thread_pool_t *init_thread_pool(int num_threads) {
    thread_pool_t *pool = malloc(sizeof(thread_pool_t));
    pool->threads = malloc(sizeof(pthread_t) * num_threads);
    pool->task_count = 0;
    pool->stop = 0;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond, NULL);
    return pool;
}

// Function to add a task to the thread pool
int add_task_to_pool(thread_pool_t *pool, void (*function)(void *), void *arg) {
    pthread_mutex_lock(&pool->lock);
    if (pool->task_count >= MAX_TASKS) {
        pthread_mutex_unlock(&pool->lock);
        return -1; // Task queue is full
    }
    // Add the task to the queue
    pool->task_queue[pool->task_count].function = function;
    pool->task_queue[pool->task_count].arg = arg;
    pool->task_count++;
    pthread_cond_signal(&pool->cond);  // Signal a thread to pick up the task
    pthread_mutex_unlock(&pool->lock);
    return 0;
}

// Worker function executed by each thread
void *worker(void *arg) {
    thread_pool_t *pool = (thread_pool_t *)arg;
    while (1) {
        pthread_mutex_lock(&pool->lock);

        while (pool->task_count == 0 && !pool->stop) {
            // No tasks, so wait for a task to arrive
            pthread_cond_wait(&pool->cond, &pool->lock);
        }

        if (pool->stop && pool->task_count == 0) {
            // If stopping and no tasks, exit
            pthread_mutex_unlock(&pool->lock);
            break;
        }

        // Get the task from the queue
        task_t task = pool->task_queue[0];
        // Shift remaining tasks
        for (int i = 1; i < pool->task_count; i++) {
            pool->task_queue[i - 1] = pool->task_queue[i];
        }
        pool->task_count--;

        pthread_mutex_unlock(&pool->lock);

        // Execute the task
        task.function(task.arg);
    }
    return NULL;
}

// Function to start the threads in the pool
void start_thread_pool(thread_pool_t *pool, int num_threads) {
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&pool->threads[i], NULL, worker, pool);
    }
}

// Function to destroy the thread pool
void destroy_thread_pool(thread_pool_t *pool) {
    pthread_mutex_lock(&pool->lock);
    pool->stop = 1;
    pthread_cond_broadcast(&pool->cond);  // Wake up all threads to exit
    pthread_mutex_unlock(&pool->lock);

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(pool->threads[i], NULL);  // Wait for all threads to finish
    }

    free(pool->threads);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->cond);
    free(pool);
}

// Example task function
void print_task(void *arg) {
    int *num = (int *)arg;
    printf("Task is being executed by thread: %d\n", *num);
    free(num);
}

int main() {
    // Create the thread pool
    thread_pool_t *pool = init_thread_pool(MAX_THREADS);

    // Start the threads in the pool
    start_thread_pool(pool, MAX_THREADS);

    // Add tasks to the pool
    for (int i = 0; i < 5; i++) {
        int *task_num = malloc(sizeof(int));
        *task_num = i + 1;
        if (add_task_to_pool(pool, print_task, task_num) == -1) {
            printf("Task queue is full\n");
        }
    }

    // Give the threads some time to complete tasks
    sleep(2);

    // Destroy the thread pool (wait for all threads to finish)
    destroy_thread_pool(pool);

    return 0;
}

