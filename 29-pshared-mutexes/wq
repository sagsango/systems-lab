#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>
#include <sys/mman.h>



using namespace std;
#define SIZE (1<<10)

struct  shared_data {
  sem_t sem;
  pthread_mutex_t mtx;
  pthread_cond_t cnd;
  int parent;
  int child;
};



// To execute C++, please define "int main()"
int main() {
    auto words = { "Hello, ", "World!", "\n" };
    for (const char* const& word : words) {
      cout << word;
    }


    void * addr = mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    if (addr == MAP_FAILED) {
      perror("mmap");
      exit(1);
    }

    struct shared_data * data = (struct shared_data*) addr;

    pthread_mutexattr_t mattr;
    pthread_condattr_t cattr;

    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&data->mtx, &mattr);


    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&data->cnd, &cattr);



    sem_init(&data->sem, 1, 1);
    data->parent = data->child = 0;

    switch (fork()) {
      case -1:
        perror("fork");
        exit(1);

      case 0:
      
        sem_wait(&data->sem);
        cout << "child:" << data->parent << endl;
        data->child = 1;
        sem_post(&data->sem);

        break;
      default:

        sem_wait(&data->sem);
        cout << "parent:" << data->child << endl;
        data->parent = 1;
        sem_post(&data->sem);

        int status;
        wait(&status);
       
    }

    
    cout << "Bye" << endl;


    return 0;
}

