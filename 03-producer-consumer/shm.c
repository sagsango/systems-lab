/* Through shared memory */
/*
    TODO: 
        1. Finish it!
        2. New file, use pthread_mutex_t and pthread_cond_t on shared memory (processes shread)

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


enum TaskType {ADD, SUB, MUL, DIV, REM, TaskTypeLimit};
struct TaskArg {
    TaskType type;
    union {
        struct add {
            int a;
            int b;
        } add;
        struct sub {
            int a;
            int b;
        } sub;
        struct mul {
            int a;
            int b;
        } mul;
        struct div {
            int a;
            int b;
        } div;
        struct rem {
            int a;
            int b;
        } rem;
    } arg;
};

enum ResultType {FAIL, SUCESS};
enum ErrorType {INVALID_OP, OVERFLOW, ZERO_DIV, NEG_REM};
struct TaskRes {
    enum ResultType type;
    union {
        struct fail {
            enum ErrorType error_code;
        } fail;
        union sucess {
           struct add_res {
                int val;
           } add_res;
           struct sub_res {
                int val;
           } sub_res;
           struct mul_res {
                int val;
           } mul_res;
           struct div_res {
                int val;
           } div_res;
           struct rem_res {
                int val;
           } rem_res;
        } sucess;
    } res;
}

struct Task {
    int id;
    struct TaskArg arg;
    struct TaskRes res;
};

struct RingBuffer {
    sem_t sem;
    int to_read, to_write;
    struct Task[N_TASKS];
};

struct RingBuffer * rb;
char init_ring_buffer() {
    rb = (struct RingBuffer*) mmap(0,
                                   sizeof(struct RingBuffer),
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED,
                                   0,
                                   0
                                );
    /* NOTE: malloc do not init the memory by 0, 
             and we dont need to init the Task memory to be 0
    */
    if (rb == NULL) {
        return 0;
    }
    rb->to_read = rb->to_write = 0;
    sem_init(&rb->sem, 1, 1);
    return 1;
}

int is_empty() {
    return rb->to_read == rb->to_write;
}

int is_full() {
    return rb->to_write + 1 == rb->to_read || (rb->to_write+1) % N_TASKS == rb->to_read;
}

struct Task* get_task() {
    sem_wait(&rb->sem);
    if (is_empty()) {
        sem_post(&rb->sem);
        return NULL;
    }
    struct Task * t = (struct Task*) malloc(sizeof(struct Task));
    if (t == NULL) {
        sem_post(&rb->sem);
        return NULL;
    }
    memcpy(t, &rb->Task[rb->to_read], sizeof(struct Task));
    rb->to_read = (rb->to_read+1) % N_TASKS;
    sem_post(&rb->sem);
    return t;
}

int add_task(Task * t) {
    sem_wait(&rb->sem);
    if (is_full()) {
        return 0;
    }
    memcpy(&rb->Task[rb->to_write], t, sizeof(struct Task));
    rb->to_write = (rb->to_write+1) % N_TASKS;
    sem_post(&rb->sem);
    return 1;
}


void producer() {
    for (int i=0; i<N_PRODUCE; ++i) {
        struct Task * t = (struct Task*) malloc(sizeof(struct Task));
        switch (i%TaskType::TaskTypeLimit) {
            case TaskType::ADD:
                t.
            case TaskType::SUB:
            case TaskType::MUL:
            case TaskType::DIV:
            case TaskType::REM:
        }
        if (t == NULL) {
            return 0;
        }
        while (!add_task(t)) {
            ;
        }
    }
}

void consumer() {
    for (int i=0; i<N_CONSUME; ++i) {
        struct Task *t;
        while ((t=get_task())) {

        }
        
    }
}

void producers() {
    pthread_t t[N_PRODUCERS];
    for (int i=0; i<N_PRODUCERS; ++i) {
        pthread_create(&t[i], 0,  producer, 0);
    }   
    for (int i=0; i<N_PRODUCERS; ++i) {
        pthread_join(&t[i], NULL);
    }
}

void consumers() {
    pthread_t t[N_CONSUMERS];
    for (int i=0; i<N_CONSUMERS; ++i) {
        pthread_create(&t[i], 0,  consumer, 0);
    }   
    for (int i=0; i<N_CONSUMERS; ++i) {
        pthread_join(&t[i], NULL);
    }
}

int main() {
    init_ring_buffer();
    int pid;
    switch((pid = fork())) {
        case -1 :
            printf("fork() failed\n");
            exit(1);
        case 0:
            consumers();
            break;
        default:
            producer();
    }
    exit(0);
}