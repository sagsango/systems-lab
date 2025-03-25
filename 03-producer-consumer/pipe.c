



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <assert.h>
#include <string.h>

#define N 100 /* NOTE: Big value of N, can cause buffer fill becase leader reader end 
                       is ideal state untill all the writes have been done by leader*/
#define READ_END 0
#define WRITE_END 1

enum task_type {ADD, SUB, MUL, DIV, NEGATE, TASK_TYPE_END};

struct task {
    int id;
    enum task_type type;
    union {
        struct { 
            int a; 
            int b;
            int result;
        } add;
        struct {
            int a;
            int b;
            int result;
        } sub;
        struct {
            int a;
            int b;
            int result;
        } mul;
        struct {
            int a;
            int b;
            int result;
        } div;
        struct {
            int a;
            int result;
        } neg;
    } payload;
    //char pad[1234]; /* We can add big padding to check read() write() are not partial */
};

int process_task(struct task *task) {
    switch (task->type) {
        case ADD: {
            task->payload.add.result = task->payload.add.a + task->payload.add.b;
            return 0;
        }
        case SUB:{
            task->payload.sub.result = task->payload.sub.a - task->payload.sub.b;
            return 0;
        }
        case MUL:{
            task->payload.mul.result = task->payload.mul.a * task->payload.mul.b;
            return 0;
        }
        case DIV:{
            task->payload.div.result = task->payload.div.a / task->payload.div.b;
            return 0;
        }
        case NEGATE:{
            task->payload.neg.result = ~task->payload.neg.a;
            return 0;
        }
        default: {
            perror("Incorrect tak type");
            exit(6);
        }
    }
    return 1;
}

int read_task(int fd, struct task * t) {
    char * buf = (char*) t;
    const int size = sizeof(struct task);
    int tot_read = 0;
    while (tot_read != size) {
        int read_bytes = read(fd, buf, size - tot_read);
        if (read_bytes < 0) {
            perror("task read failed");
            exit(4);
        }
        if (read_bytes == 0) {
            return 0; // pipe closed
        }
        buf += read_bytes;
        tot_read += read_bytes;
    }
    return size;
}

int write_task(int fd, struct task * t) {
    char * buf = (char*) t;
    const int size = sizeof(struct task);
    int tot_write = 0;
    while (tot_write != size) {
        int write_bytes = write(fd, buf, size - tot_write);
        if (write_bytes < 0) {
            perror("task read failed");
            exit(5);
        }
        if (write_bytes == 0) {
            return 0; // pipe closed
        }
        buf += write_bytes;
        tot_write += write_bytes;
    }
    return size;
}

struct leader_data {
    int read_fd;
    int write_fd;
    struct task tasks[N];
};

struct worker_data {
    int read_fd;
    int write_fd;
};

struct worker_data w_data;
struct leader_data l_data;

int leader(struct leader_data * data) {

    // Submit the jobs
    for (int i=0; i<N; ++i) {
        data->tasks[i].id = i;
        switch(i%TASK_TYPE_END) {
            case ADD: {
                data->tasks[i].type = ADD;
                data->tasks[i].payload.add.a = i+1; 
                data->tasks[i].payload.add.b = i-1; 
                break;
            }
            case SUB: {
                data->tasks[i].type = SUB;
                data->tasks[i].payload.sub.a = i+2; 
                data->tasks[i].payload.sub.b = i-2; 
                break;
            }
            case MUL: {
                data->tasks[i].type = MUL;
                data->tasks[i].payload.mul.a = i+3; 
                data->tasks[i].payload.mul.b = i-3; 
                break;
            }
            case DIV: {
                data->tasks[i].type = DIV;
                data->tasks[i].payload.div.a = i+4; 
                data->tasks[i].payload.div.b = i-4; 
                break;
            }
            case NEGATE: {
                data->tasks[i].type = NEGATE;
                data->tasks[i].payload.neg.a = i+5; 
                break;
            }
            default: {
                perror("Unexpected task type");
                exit(6);
            }
        }
        write_task(data->write_fd, &(data->tasks[i]));
        printf("Writter");
    }
    close(data->write_fd);
    printf("Writte Done!!");

    // Read the job result & assert the results
    int task_done = 0;
    struct task completed;
    while (read_task(data->read_fd, &completed)  == sizeof(struct task)) {
        task_done += 1;
        process_task(&(data->tasks[completed.id]));
        //assert (comepleted.result == data->tasks[completed.id].result )
        assert (memcmp(&completed, &(data->tasks[completed.id]), sizeof(struct task)) == 0);
    }
    close(data->read_fd);
    printf("Reader Done!!");
    printf("total result received: %d", task_done);
    return 0;
}

int worker(struct worker_data *data) {
    //int job_done = 0;
    struct task todo;
    while (read_task(data->read_fd, &todo)  == sizeof(struct task)) {
        printf("Read");
        process_task(&todo);
        write_task(data->write_fd, &todo);
        //assert (comepleted->result == data->tasks[compeleted->id].result )
    }
    close(data->read_fd);
    close(data->write_fd);
    //printf("total result received: %d", task_done);
    return 0;
}


int main() {
    int fd_pc[2]; /*from producer to consumer*/
    int fd_cp[2]; /*from consumer to producer*/
    int read_fd, write_fd;

    if (pipe(fd_pc)) {
        perror("pipe(fd_pc) failed");
        exit(1); // pi
    }
    if (pipe(fd_cp)) {
        perror("pipe(fd_cp) failed");
        exit(2);
    }

    int child_pid;

    switch ((child_pid = fork())) {
        case -1: {
            perror("fork() failed");
            exit(3);
        }
        case 0: {
            // child is a worker
            // It will compelete the task 
            // and send the result back to parent

            // consumer
            close(fd_pc[WRITE_END]);
            w_data.read_fd = fd_pc[READ_END];
            close(fd_cp[READ_END]);
            w_data.write_fd = fd_cp[WRITE_END];
            
            worker(&w_data);
            printf("Child says Bye!\n");
            break;
        }
        default: { // 
           
            // producer
            close(fd_pc[READ_END]);
            l_data.write_fd = fd_pc[WRITE_END];
            close(fd_cp[WRITE_END]);
            l_data.read_fd = fd_cp[READ_END];

            leader(&l_data);
            printf("Parent says Bye!\n");
            break;
        }
    }
}