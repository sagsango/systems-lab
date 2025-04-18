/* Fixed-Size Thread Pool (with Task Batching) */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4          // Number of threads in the pool
#define QUEUE_SIZE 20          // Maximum number of tasks in the queue
#define BATCH_SIZE 5           // Number of tasks to process in each batch

// Task structure
typedef struct {
    void (*task_function)(void*);
    void *arg;
} Task;

// Task Queue structure
typedef struct {
    Task tasks[QUEUE_SIZE];
    int front;
    int rear;
    pthread_mutex_t queueLock;  // Mutex to ensure thread-safety for the queue
    pthread_cond_t cond;       // Condition variable to signal workers
} TaskQueue;

// Worker structure to track the worker's state
typedef struct {
    pthread_t thread_id;
    TaskQueue *taskQueue;  // Pointer to the shared task queue
    int is_active;
} Worker;

// Global pool of workers
Worker workerPool[NUM_THREADS];
TaskQueue taskQueue;  // Shared task queue for all workers

// Function to add a task to the queue
void addTaskToQueue(void (*task_function)(void*), void *arg) {
    pthread_mutex_lock(&taskQueue.queueLock);

    // Wait if the queue is full
    while ((taskQueue.rear + 1) % QUEUE_SIZE == taskQueue.front) {
        pthread_cond_wait(&taskQueue.cond, &taskQueue.queueLock);
    }

    // Add the task to the queue
    taskQueue.tasks[taskQueue.rear].task_function = task_function;
    taskQueue.tasks[taskQueue.rear].arg = arg;
    taskQueue.rear = (taskQueue.rear + 1) % QUEUE_SIZE;

    pthread_cond_signal(&taskQueue.cond);  // Signal that there is a new task
    pthread_mutex_unlock(&taskQueue.queueLock);
}

// Function to get a batch of tasks from the queue
int getTaskBatchFromQueue(Task *batch[]) {
    pthread_mutex_lock(&taskQueue.queueLock);

    // Wait if the queue is empty
    while (taskQueue.front == taskQueue.rear) {
        pthread_cond_wait(&taskQueue.cond, &taskQueue.queueLock);
    }

    // Determine the number of tasks in the batch (up to BATCH_SIZE)
    int batchSize = 0;
    while (batchSize < BATCH_SIZE && taskQueue.front != taskQueue.rear) {
        batch[batchSize] = &taskQueue.tasks[taskQueue.front];
        taskQueue.front = (taskQueue.front + 1) % QUEUE_SIZE;
        batchSize++;
    }

    pthread_mutex_unlock(&taskQueue.queueLock);
    return batchSize;
}

// Worker thread function to process tasks in batches
void* workerThread(void *arg) {
    Worker *worker = (Worker*)arg;

    while (1) {
        Task *batch[BATCH_SIZE];
        int batchSize = getTaskBatchFromQueue(batch);

        // Process each task in the batch
        for (int i = 0; i < batchSize; i++) {
            batch[i]->task_function(batch[i]->arg);
        }
    }
    return NULL;
}

// A sample task function
void printTask(void *arg) {
    int *num = (int*)arg;
    printf("Executing task: %d\n", *num);
    free(arg);  // Free the memory allocated for the task argument
}

int main() {
    // Initialize the shared task queue
    taskQueue.front = 0;
    taskQueue.rear = 0;
    pthread_mutex_init(&taskQueue.queueLock, NULL);
    pthread_cond_init(&taskQueue.cond, NULL);

    // Create worker threads
    for (int i = 0; i < NUM_THREADS; i++) {
        workerPool[i].taskQueue = &taskQueue;  // Assign the shared task queue
        pthread_create(&workerPool[i].thread_id, NULL, workerThread, (void*)&workerPool[i]);
    }

    // Add tasks to the thread pool
    for (int i = 0; i < 20; i++) {
        int *taskData = malloc(sizeof(int));
        if (!taskData) {
            perror("Memory allocation failed");
            exit(1);
        }
        *taskData = i;
        addTaskToQueue(printTask, taskData);  // Add task to the shared queue
    }

    // Allow time for threads to finish processing tasks
    sleep(2);  // Adjust time based on the number of tasks and worker speed

    return 0;
}

