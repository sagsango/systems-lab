/* Dynamic number of threads based on number of tasks */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define QUEUE_SIZE 10  // Maximum number of tasks in the queue
#define MAX_THREADS 4  // Maximum number of threads at any given point
#define IDLE_TIME 2    // Time in seconds to wait before thread terminates (idle time)

// Task structure that stores the function pointer and the argument for the task
typedef struct {
    void (*task_function)(void*);
    void *arg;
} Task;

// A simple task queue structure
typedef struct {
    Task tasks[QUEUE_SIZE];
    int front;
    int rear;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} TaskQueue;

// Worker thread state to track the thread's idle state
typedef struct {
    pthread_t thread_id;
    int is_active;  // 1 if thread is processing, 0 if idle and should terminate
    time_t last_task_time;  // Time when the thread last processed a task
} Worker;

// Global task queue and worker pool
TaskQueue taskQueue = {.front = 0, .rear = 0, .lock = PTHREAD_MUTEX_INITIALIZER, .cond = PTHREAD_COND_INITIALIZER};
Worker workerPool[MAX_THREADS];

// Function to add a task to the queue
void addTask(void (*task_function)(void*), void *arg) {
    pthread_mutex_lock(&taskQueue.lock);

    // If the queue is full, we wait
    if ((taskQueue.rear + 1) % QUEUE_SIZE == taskQueue.front) {
        pthread_cond_wait(&taskQueue.cond, &taskQueue.lock);
    }

    // Add the task to the queue
    taskQueue.tasks[taskQueue.rear].task_function = task_function;
    taskQueue.tasks[taskQueue.rear].arg = arg;
    taskQueue.rear = (taskQueue.rear + 1) % QUEUE_SIZE;

    // Signal that a task is available
    pthread_cond_signal(&taskQueue.cond);

    pthread_mutex_unlock(&taskQueue.lock);
}

// Function to get a task from the queue
Task getTask() {
    pthread_mutex_lock(&taskQueue.lock);

    // If the queue is empty, we wait
    while (taskQueue.front == taskQueue.rear) {
        pthread_cond_wait(&taskQueue.cond, &taskQueue.lock);
    }

    Task task = taskQueue.tasks[taskQueue.front];
    taskQueue.front = (taskQueue.front + 1) % QUEUE_SIZE;

    pthread_mutex_unlock(&taskQueue.lock);
    return task;
}

// Function to process tasks
void* worker(void *arg) {
    Worker *worker = (Worker*)arg;

    while (1) {
        // Get a task from the queue
        Task task = getTask();
        
        // Update worker state: task was assigned
        worker->is_active = 1;
        worker->last_task_time = time(NULL);
        
        // Execute the task
        task.task_function(task.arg);

        // After processing the task, check if it should terminate after idle
        worker->is_active = 0;
        if (time(NULL) - worker->last_task_time >= IDLE_TIME) {
            printf("Thread %ld terminating after being idle for %d seconds\n", worker->thread_id, IDLE_TIME);
            pthread_exit(NULL);  // Terminate the worker thread
        }
    }
    return NULL;
}

// Function to check if there are inactive threads and create new ones if needed
void manageThreads() {
    int activeThreads = 0;

    for (int i = 0; i < MAX_THREADS; i++) {
        if (workerPool[i].is_active) {
            activeThreads++;
        }
    }

    // If no active threads, create a new thread
    if (activeThreads == 0) {
        for (int i = 0; i < MAX_THREADS; i++) {
            if (workerPool[i].is_active == 0) {
                workerPool[i].is_active = 1;
                pthread_create(&workerPool[i].thread_id, NULL, worker, (void*)&workerPool[i]);
                printf("Created new worker thread: %ld\n", workerPool[i].thread_id);
                break;
            }
        }
    }
}

// A sample task function
void printTask(void *arg) {
    int *num = (int*)arg;
    printf("Executing task: %d\n", *num);
    free(arg);  // Free memory allocated for the task argument
}

int main() {
    // Initialize all workers to be inactive
    for (int i = 0; i < MAX_THREADS; i++) {
        workerPool[i].is_active = 0;
    }

    // Add some tasks to the queue
    for (int i = 0; i < 10; i++) {
        int *taskData = malloc(sizeof(int));
        if (!taskData) {
            perror("Memory allocation failed");
            exit(1);
        }
        *taskData = i;
        addTask(printTask, taskData);  // Add a task to the queue
        manageThreads();  // Check if we need to spawn a new thread
    }

    // Wait for tasks to complete (allow time for threads to finish)
    sleep(5);  // You can adjust the sleep time or implement a more robust solution

    return 0;
}

