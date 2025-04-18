// TODO:


#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <stdatomic.h>
#include <unistd.h>

#define SIZE (5)
#define N (20)
#define WAIT_TIME (10)

struct queue {
	int to_read, to_write;
	int data[SIZE];
	int (*push)(int);
	int (*pop)();
	int (*empty)();
	int (*full)();
};

struct queue q;
int task_produced, task_exicuted;
atomic_int done;
pthread_mutex_t q_mtx;
pthread_cond_t not_full, not_empty;

int q_push(int e) {
	q.data[q.to_write] = e;
	q.to_write += 1;
	q.to_write %= SIZE;
}

int q_pop() {
	int e = q.data[q.to_read];
	q.to_read += 1;
	q.to_read %= SIZE;
	return e;
}

int q_empty() {
	return q.to_read == q.to_write;
}

int q_full() {
	return (q.to_write+1) % SIZE == q.to_read;
}

void init_q() {
	q.push = q_push;
	q.pop = q_pop;
	q.empty = q_empty;
	q.full = q_full;
}

void * producer(void * arg) {
	for (;;) {
		pthread_mutex_lock(&q_mtx);
		while (!done && q.full()) {
			pthread_cond_wait(&not_full, &q_mtx);
		}
		if (done) {
			pthread_cond_broadcast(&not_empty);
			pthread_cond_broadcast(&not_full);
			pthread_mutex_unlock(&q_mtx);
			break;
		}
		int id = pthread_self();
		q.push(id);
		task_produced += 1;
		pthread_cond_broadcast(&not_empty);
		pthread_mutex_unlock(&q_mtx);
	}
}

void * consumer(void * arg) {
	for (;;) {
		pthread_mutex_lock(&q_mtx);
		while (!done && q.empty()) {
			pthread_cond_wait(&not_empty, &q_mtx);
		}
		if (done && q.empty()) {
			pthread_cond_broadcast(&not_full);
			pthread_cond_broadcast(&not_empty);
			pthread_mutex_unlock(&q_mtx);
			break;
		}
		int task = q.pop();
		task_exicuted += 1;
		pthread_cond_broadcast(&not_full);
		pthread_mutex_unlock(&q_mtx);
	}
}

int main() {
	init_q();
  pthread_t p[N], c[N];

	pthread_mutex_init(&q_mtx, 0);
	pthread_cond_init(&not_full, 0);
	pthread_cond_init(&not_empty, 0);

	for (int i=0; i<N; ++i) {
			pthread_create(&p[i], 0, producer, 0);
			pthread_create(&c[i], 0, consumer, 0);
	}

	sleep(WAIT_TIME);
	done = 1;

	for (int i=0; i<N; ++i) {
		pthread_join(p[i], 0);
		pthread_join(c[i], 0);
	}

	assert (task_produced == task_exicuted);
	printf("Done (%d=%d)\n", task_produced, task_exicuted);
}
