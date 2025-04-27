#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

#define MAX_NUM 100
/*
 *
 * This Program shows the idea how userland thread can be created
 * and can be scheduled.
 *
 *
 * We have 2 user level thread
 *  1. Prints odd numbers
 *  2. Prints even numbers
 * The main program is going to swith back and forth from one to another
 * (For simlicity we have switched from odd to even back and forth)
 */

ucontext_t main_context, even_context, odd_context;

/*
 * 1st userland thread
 */
void print_even() {
    for (int i = 2; i <= MAX_NUM; i += 2) {
        printf("%d ", i);
        swapcontext(&even_context, &odd_context);  /* Switch to userland thread 2 */
    }
    /* Switch to main thread  : NOT needed*/
    //swapcontext(&even_context, &main_context);
}

/*
 * 2nd userland thread
 */
void print_odd() {
    for (int i = 1; i <= MAX_NUM; i += 2) {
        printf("%d ", i);
        swapcontext(&odd_context, &even_context);  /* Switch to userland thread 1 */
    }
    /* Switch to main thread i: NOT Needed*/
    //swapcontext(&odd_context, &main_context);
}

int main() {
    /* Stack for userland threads */
    char even_stack[1024 * 128];
    char odd_stack[1024 * 128];

    /*
     * Initialize the 1st thread context
     */
    getcontext(&even_context);
    even_context.uc_stack.ss_sp = even_stack;
    even_context.uc_stack.ss_size = sizeof(even_stack);
    even_context.uc_link = &main_context;  /* When done, go back to main context */
    makecontext(&even_context, print_even, 0);

    /*
     * Initialize the odd context
     */
    getcontext(&odd_context);
    odd_context.uc_stack.ss_sp = odd_stack;
    odd_context.uc_stack.ss_size = sizeof(odd_stack);
    odd_context.uc_link = &main_context;  /* When done, go back to main context */
    makecontext(&odd_context, print_odd, 0);

    /* Start the process by swapping to the odd context */
    swapcontext(&main_context, &odd_context);  /* Start with 1st thread numbers, and save the main thread context */

    /* Print a message when both functions are done */
    printf("\nAll numbers from 1 to 100 printed!\n");

    return 0;
}
