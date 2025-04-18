#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4
#define MAX_TASK_QUEUE_SIZE 10

// Define Task Struct
typedef struct {
    void (*task_function)(void* arg);  // Task function to execute
    void *arg;                         // Argument for the task
    int cancel_flag;                   // Flag to indicate task cancellation
} Task;

// Define Worker Struct
typedef struct {
    pthread_t thread_id;
    int is_active;
} Worker;

// Task Queue Structure
typedef struct {
    Task *tasks[MAX_TASK_QUEUE_SIZE];
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TaskQueue;

// Thread Pool Structure
typedef struct {
    Worker workers[NUM_THREADS];
    TaskQueue task_queue;
    int shutdown_flag; // Graceful shutdown flag
    pthread_mutex_t shutdown_mutex;
} ThreadPool;

// Priority Queue Operations
void init_task_queue(TaskQueue *queue) {
    queue->size = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

int is_task_queue_empty(TaskQueue *queue) {
    return queue->size == 0;
}

int is_task_queue_full(TaskQueue *queue) {
    return queue->size == MAX_TASK_QUEUE_SIZE;
}

void add_task_to_queue(TaskQueue *queue, void (*task_function)(void*), void *arg) {
    if (is_task_queue_full(queue)) {
        printf("Task queue is full!\n");
        return;
    }

    Task *new_task = (Task*)malloc(sizeof(Task));
    new_task->task_function = task_function;
    new_task->arg = arg;
    new_task->cancel_flag = 0;

    pthread_mutex_lock(&queue->mutex);
    queue->tasks[queue->size++] = new_task;
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

Task* pop_task_from_queue(TaskQueue *queue) {
    if (is_task_queue_empty(queue)) {
        return NULL;
    }

    pthread_mutex_lock(&queue->mutex);
    Task *task = queue->tasks[0];

    // Shift the remaining tasks down to fill the gap
    for (int i = 1; i < queue->size; i++) {
        queue->tasks[i-1] = queue->tasks[i];
    }
    queue->size--;

    pthread_mutex_unlock(&queue->mutex);

    return task;
}

// Worker Thread Function
void* worker_thread(void *arg) {
    ThreadPool *pool = (ThreadPool*)arg;

    while (1) {
        pthread_mutex_lock(&pool->task_queue.mutex);

        // Check if shutdown flag is set
        if (pool->shutdown_flag) {
            pthread_mutex_unlock(&pool->task_queue.mutex);
            break;  // Exit the loop and stop the thread
        }

        while (is_task_queue_empty(&pool->task_queue)) {
            pthread_cond_wait(&pool->task_queue.cond, &pool->task_queue.mutex);
        }

        Task* task = pop_task_from_queue(&pool->task_queue);
        
        if (task != NULL && task->cancel_flag == 0) {
            // Execute the task if not canceled
            task->task_function(task->arg);
            free(task);
        } else {
            // Task was canceled, free it
            free(task);
        }

        pthread_mutex_unlock(&pool->task_queue.mutex);
    }

    return NULL;
}

// Thread Pool Initialization
void init_thread_pool(ThreadPool *pool) {
    pool->shutdown_flag = 0;
    pthread_mutex_init(&pool->shutdown_mutex, NULL);

    init_task_queue(&pool->task_queue);

    for (int i = 0; i < NUM_THREADS; i++) {
        pool->workers[i].is_active = 1;
        pthread_create(&pool->workers[i].thread_id, NULL, worker_thread, (void*)pool);
    }
}

// Task Cancellation
void cancel_task(Task *task) {
    task->cancel_flag = 1;
}

// Graceful Shutdown of Thread Pool
void shutdown_thread_pool(ThreadPool *pool) {
    pthread_mutex_lock(&pool->shutdown_mutex);
    pool->shutdown_flag = 1;

    // Wake up all threads to make them exit
    pthread_mutex_lock(&pool->task_queue.mutex);
    pthread_cond_broadcast(&pool->task_queue.cond);
    pthread_mutex_unlock(&pool->task_queue.mutex);

    // Join all worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(pool->workers[i].thread_id, NULL);
    }

    pthread_mutex_unlock(&pool->shutdown_mutex);
}

// Example Task Function
void example_task(void *arg) {
    int *num = (int*)arg;
    printf("Task started with argument: %d\n", *num);
    sleep(2);  // Simulate work
    printf("Task completed with argument: %d\n", *num);
}

// Main Function
int main() {
    ThreadPool pool;
    init_thread_pool(&pool);

    // Submit tasks
    for (int i = 0; i < 5; i++) {
        int *arg = (int*)malloc(sizeof(int));
        *arg = i;
        add_task_to_queue(&pool.task_queue, example_task, arg);
    }

    // Simulate canceling some tasks
    sleep(1);  // Allow some tasks to start
    printf("Cancelling task with argument: 2\n");
    cancel_task(pool.task_queue.tasks[2]);

    // Gracefully shutdown the thread pool after a delay
    sleep(5);
    printf("Shutting down thread pool.\n");
    shutdown_thread_pool(&pool);

    return 0;
}

