#include<stdio.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mtx;

int main () {


    pthread_mutex_init(&mtx, 0);


    if (pthread_mutex_trylock(&mtx) == 0) {
      printf("Got the lock\n");
    }

    if (pthread_mutex_trylock(&mtx) == 0) {
      assert (0); // This should not happen
    } else {
      printf("Did not get the lock\n");
    }


    pthread_mutex_unlock(&mtx);

    pthread_mutex_destroy(&mtx);

}
