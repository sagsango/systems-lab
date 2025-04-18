/* Thread Pool with Futures/Promises (Task Result Handling)  */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define TASK_QUEUE_SIZE 10

// Define Task Struct
typedef struct {
    void (*task_function)(void* arg);  // Task function to execute
    void *arg;                         // Argument for the task
    int is_completed;                  // Flag indicating if the task is completed
    void *result;                      // Task result (will be filled by promise)
    pthread_mutex_t mutex;             // Mutex to synchronize access to result
    pthread_cond_t cond;               // Condition variable to wait for result
} Task;

// Define Worker Struct
typedef struct {
    pthread_t thread_id;
    int is_active;
} Worker;

// Define ThreadPool Struct
typedef struct {
    Worker workers[NUM_THREADS];
    Task task_queue[TASK_QUEUE_SIZE];
    int front;
    int rear;
    pthread_mutex_t queue_mutex;
    pthread_cond_t queue_cond;
    pthread_cond_t task_completed_cond;
} ThreadPool;

// Task Queue Operations
void init_task_queue(ThreadPool *pool) {
    pool->front = 0;
    pool->rear = 0;
    pthread_mutex_init(&pool->queue_mutex, NULL);
    pthread_cond_init(&pool->queue_cond, NULL);
    pthread_cond_init(&pool->task_completed_cond, NULL);
}

int is_task_queue_empty(ThreadPool *pool) {
    return pool->front == pool->rear;
}

int is_task_queue_full(ThreadPool *pool) {
    return ((pool->rear + 1) % TASK_QUEUE_SIZE) == pool->front;
}

void add_task_to_queue(ThreadPool *pool, void (*task_function)(void*), void *arg) {
    if (is_task_queue_full(pool)) {
        printf("Task queue is full!\n");
        return;
    }
    
    pthread_mutex_lock(&pool->queue_mutex);

    pool->task_queue[pool->rear].task_function = task_function;
    pool->task_queue[pool->rear].arg = arg;
    pool->task_queue[pool->rear].is_completed = 0;
    pool->task_queue[pool->rear].result = NULL;
    pthread_mutex_init(&pool->task_queue[pool->rear].mutex, NULL);
    pthread_cond_init(&pool->task_queue[pool->rear].cond, NULL);
    pool->rear = (pool->rear + 1) % TASK_QUEUE_SIZE;

    pthread_cond_signal(&pool->queue_cond);
    pthread_mutex_unlock(&pool->queue_mutex);
}

Task* get_task_from_queue(ThreadPool *pool) {
    if (is_task_queue_empty(pool)) {
        return NULL;
    }

    pthread_mutex_lock(&pool->queue_mutex);

    Task* task = &pool->task_queue[pool->front];
    pool->front = (pool->front + 1) % TASK_QUEUE_SIZE;

    pthread_mutex_unlock(&pool->queue_mutex);
    return task;
}

// Worker Thread Function
void* worker_thread(void *arg) {
    ThreadPool *pool = (ThreadPool*)arg;

    while (1) {
        pthread_mutex_lock(&pool->queue_mutex);

        while (is_task_queue_empty(pool)) {
            pthread_cond_wait(&pool->queue_cond, &pool->queue_mutex);
        }

        Task* task = get_task_from_queue(pool);
        if (task != NULL) {
            // Execute task
            task->task_function(task->arg);

            // Mark task as completed and notify the main thread
            pthread_mutex_lock(&task->mutex);
            task->is_completed = 1;
            pthread_cond_signal(&task->cond);
            pthread_mutex_unlock(&task->mutex);
        }

        pthread_mutex_unlock(&pool->queue_mutex);
    }

    return NULL;
}

// Thread Pool Initialization
void init_thread_pool(ThreadPool *pool) {
    for (int i = 0; i < NUM_THREADS; i++) {
        pool->workers[i].is_active = 1;
        pthread_create(&pool->workers[i].thread_id, NULL, worker_thread, (void*)pool);
    }
}

// Promise: Set the result of a task
void promise_set_result(Task *task, void *result) {
    pthread_mutex_lock(&task->mutex);
    task->result = result;
    pthread_mutex_unlock(&task->mutex);
}

// Future: Get the result of a task (blocking call)
void* future_get_result(Task *task) {
    pthread_mutex_lock(&task->mutex);

    while (!task->is_completed) {
        pthread_cond_wait(&task->cond, &task->mutex);
    }

    void *result = task->result;

    pthread_mutex_unlock(&task->mutex);
    return result;
}

// Example task function
void example_task(void *arg) {
    int *num = (int*)arg;
    printf("Task started with argument: %d\n", *num);
    sleep(2);  // Simulate work
    printf("Task completed with argument: %d\n", *num);
    *num *= 2;  // Set the result
}

int main() {
    ThreadPool pool;
    init_task_queue(&pool);
    init_thread_pool(&pool);

    // Submit tasks
    for (int i = 0; i < 5; i++) {
        int *arg = (int*)malloc(sizeof(int));
        *arg = i;
        add_task_to_queue(&pool, example_task, arg);
    }

    // Wait for tasks to complete and get results
    for (int i = 0; i < 5; i++) {
        Task* task = &pool.task_queue[(pool.front + i) % TASK_QUEUE_SIZE];
        int *result = (int*)future_get_result(task);
        printf("Task result: %d\n", *result);
        free(result);  // Clean up the result
    }

    // Clean up threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cancel(pool.workers[i].thread_id);
        pthread_join(pool.workers[i].thread_id, NULL);
    }

    return 0;
}

