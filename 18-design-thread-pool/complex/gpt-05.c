/* Thread Pool with Priority Queue (Task Prioritization) */
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
    int priority;                      // Task priority (lower number = higher priority)
} Task;

// Define Worker Struct
typedef struct {
    pthread_t thread_id;
    int is_active;
} Worker;

// Priority Queue Structure (Min Heap)
typedef struct {
    Task *tasks[MAX_TASK_QUEUE_SIZE];
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} PriorityQueue;

// ThreadPool Structure
typedef struct {
    Worker workers[NUM_THREADS];
    PriorityQueue task_queue;
} ThreadPool;

// Priority Queue Operations
void init_priority_queue(PriorityQueue *pq) {
    pq->size = 0;
    pthread_mutex_init(&pq->mutex, NULL);
    pthread_cond_init(&pq->cond, NULL);
}

int is_priority_queue_empty(PriorityQueue *pq) {
    return pq->size == 0;
}

int is_priority_queue_full(PriorityQueue *pq) {
    return pq->size == MAX_TASK_QUEUE_SIZE;
}

void swap_tasks(Task **a, Task **b) {
    Task *temp = *a;
    *a = *b;
    *b = temp;
}

void heapify_up(PriorityQueue *pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->tasks[parent]->priority <= pq->tasks[index]->priority) {
            break;
        }
        swap_tasks(&pq->tasks[parent], &pq->tasks[index]);
        index = parent;
    }
}

void heapify_down(PriorityQueue *pq, int index) {
    int left_child, right_child, smallest;

    while (index < pq->size) {
        left_child = 2 * index + 1;
        right_child = 2 * index + 2;
        smallest = index;

        if (left_child < pq->size && pq->tasks[left_child]->priority < pq->tasks[smallest]->priority) {
            smallest = left_child;
        }

        if (right_child < pq->size && pq->tasks[right_child]->priority < pq->tasks[smallest]->priority) {
            smallest = right_child;
        }

        if (smallest == index) {
            break;
        }

        swap_tasks(&pq->tasks[index], &pq->tasks[smallest]);
        index = smallest;
    }
}

void add_task_to_priority_queue(PriorityQueue *pq, void (*task_function)(void*), void *arg, int priority) {
    if (is_priority_queue_full(pq)) {
        printf("Priority Queue is full!\n");
        return;
    }

    Task *new_task = (Task*)malloc(sizeof(Task));
    new_task->task_function = task_function;
    new_task->arg = arg;
    new_task->priority = priority;

    pthread_mutex_lock(&pq->mutex);

    pq->tasks[pq->size] = new_task;
    pq->size++;
    heapify_up(pq, pq->size - 1);

    pthread_cond_signal(&pq->cond);
    pthread_mutex_unlock(&pq->mutex);
}

Task* pop_task_from_priority_queue(PriorityQueue *pq) {
    if (is_priority_queue_empty(pq)) {
        return NULL;
    }

    pthread_mutex_lock(&pq->mutex);

    Task *task = pq->tasks[0];

    pq->tasks[0] = pq->tasks[pq->size - 1];
    pq->size--;
    heapify_down(pq, 0);

    pthread_mutex_unlock(&pq->mutex);

    return task;
}

// Worker Thread Function
void* worker_thread(void *arg) {
    ThreadPool *pool = (ThreadPool*)arg;

    while (1) {
        pthread_mutex_lock(&pool->task_queue.mutex);

        while (is_priority_queue_empty(&pool->task_queue)) {
            pthread_cond_wait(&pool->task_queue.cond, &pool->task_queue.mutex);
        }

        Task* task = pop_task_from_priority_queue(&pool->task_queue);

        if (task != NULL) {
            task->task_function(task->arg);
            free(task);
        }

        pthread_mutex_unlock(&pool->task_queue.mutex);
    }

    return NULL;
}

// Thread Pool Initialization
void init_thread_pool(ThreadPool *pool) {
    init_priority_queue(&pool->task_queue);

    for (int i = 0; i < NUM_THREADS; i++) {
        pool->workers[i].is_active = 1;
        pthread_create(&pool->workers[i].thread_id, NULL, worker_thread, (void*)pool);
    }
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

    // Submit tasks with different priorities
    for (int i = 0; i < 5; i++) {
        int *arg = (int*)malloc(sizeof(int));
        *arg = i;
        add_task_to_priority_queue(&pool.task_queue, example_task, arg, rand() % 10);
    }

    // Allow some time for threads to finish their tasks
    sleep(10);

    // Clean up threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cancel(pool.workers[i].thread_id);
        pthread_join(pool.workers[i].thread_id, NULL);
    }

    return 0;
}

