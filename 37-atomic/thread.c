/*
 * https://en.cppreference.com/w/c/thread
 */

#include <threads.h>
#include <stdio.h>

int tf(void*arg) {
    printf("Hello World\n");
    return 0;
}
int main() {
    thrd_t th;
    thrd_create(&th, tf, NULL);
    thrd_join(th, NULL);

}