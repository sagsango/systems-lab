#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 4        // Number of threads in the pool
#define QUEUE_SIZE 10        // Maximum number of tasks in each thread's queue
#define NUM_TASKS 20         // Number of tasks to process

// Task structure
typedef struct {
	void (*task_function)(void*);
	void *arg;
} Task;

// Thread-specific task queue structure
typedef struct {
	Task tasks[QUEUE_SIZE];
	int front;
	int rear;
	pthread_mutex_t queueLock;  // Mutex to ensure thread-safety for each queue
	pthread_cond_t cond;
} TaskQueue;

// Worker structure to track the worker's state
typedef struct {
	pthread_t thread_id;
	TaskQueue taskQueue;  // Directly include TaskQueue in the Worker
	int is_active;
} Worker;

// Global pool of workers
Worker workerPool[NUM_THREADS];

// Function to add a task to a worker's queue
void addTaskToQueue(int worker_id, void (*task_function)(void*), void *arg) {
	Worker *worker = &workerPool[worker_id];  // Access the worker's queue
	TaskQueue *queue = &worker->taskQueue;    // Access the worker's task queue

	pthread_mutex_lock(&queue->queueLock);  // Lock the queue for safe access

	// Wait if the queue is full
	while ((queue->rear + 1) % QUEUE_SIZE == queue->front) {
		pthread_cond_wait(&queue->cond, &queue->queueLock);
	}

	// Add the task to the queue
	queue->tasks[queue->rear].task_function = task_function;
	queue->tasks[queue->rear].arg = arg;
	queue->rear = (queue->rear + 1) % QUEUE_SIZE;

	// Signal that there is a new task
	pthread_cond_signal(&queue->cond);
	pthread_mutex_unlock(&queue->queueLock);  // Unlock the queue
}

// Function to get a task from the worker's queue
Task getTaskFromQueue(int worker_id) {
	Worker *worker = &workerPool[worker_id];  // Access the worker's queue
	TaskQueue *queue = &worker->taskQueue;    // Access the worker's task queue

	pthread_mutex_lock(&queue->queueLock);  // Lock the queue for safe access

	// Wait if the queue is empty
	while (queue->front == queue->rear) {
		pthread_cond_wait(&queue->cond, &queue->queueLock);
	}

	Task task = queue->tasks[queue->front];
	queue->front = (queue->front + 1) % QUEUE_SIZE;

	pthread_mutex_unlock(&queue->queueLock);  // Unlock the queue
	return task;
}

bool isEmptyTaskQueue(id) {
	bool is_empty;
	Worker *worker = &workerPool[worker_id];
	TaskQueue *queue = &worker->taskQueue;
	pthread_mutex_lock(&queue->queueLock);
	is_empty = queue->front == queue->rear;
	pthread_mutex_unlock(&queue->queueLock);
	return is_empty;

}

// Function for the worker threads to execute tasks
void* workerThread(void *arg) {
	int id = (int)(unsigned long)arg;
	int other = 0; // id of other thread to which we will steel the work
	Task task;

	while (1) {
		// Try to get a task from the worker's own task queue

		if (!isEmptyTaskQueue(id)) {
			task = getTaskFromQueue(id);
		} else {
			for (int times=0; times<NUM_THREADS; ++times) {
				if (i != other && !isEmptyTaskQueue(other)) {
					task = getTaskFromQueue(other)
						break;
				}
				other = (other + 1) % NUM_THREADS;
			}
		}

		// If task is retrieved, execute it
		task.task_function(task.arg);
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
	// Initialize worker pool and task queues
	for (int i = 0; i < NUM_THREADS; i++) {
		Worker *worker = &workerPool[i];
		worker->taskQueue.front = 0;
		worker->taskQueue.rear = 0;
		pthread_mutex_init(&worker->taskQueue.queueLock, NULL);
		pthread_cond_init(&worker->taskQueue.cond, NULL);
		worker->is_active = 0;
	}

	// Create worker threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&workerPool[i].thread_id, NULL, workerThread, (void*)(unsigned long)(i));
	}

	// Add tasks to the thread pool
	for (int i = 0; i < NUM_TASKS; i++) {
		int *taskData = malloc(sizeof(int));
		if (!taskData) {
			perror("Memory allocation failed");
			exit(1);
		}
		*taskData = i;
		addTaskToQueue(i % NUM_THREADS, printTask, taskData);  // Distribute tasks to worker queues
	}

	// Wait for tasks to be completed (you can improve this with proper thread joining or monitoring)
	sleep(5);  // Adjust time depending on the task complexity

	return 0;
}

