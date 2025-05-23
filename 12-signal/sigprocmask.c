#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    sigset_t new_mask, old_mask;

    /*
     * Initialize an empty set and add SIGINT (Ctrl+C)
     */
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGINT);

    /* 
     * Block SIGINT
     * 
     */
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        perror("sigprocmask");
        return 1;
    }

    printf("SIGINT is now blocked. Try pressing Ctrl+C...\n");
    sleep(5);  /* SIGINT will be blocked during this time */

    /*
     *  Restore old signal mask (unblock SIGINT)
     */
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        perror("sigprocmask");
        return 1;
    }

    printf("SIGINT unblocked. Now Ctrl+C will terminate the program.\n");
    sleep(5);  /* Now SIGINT can interrupt */


    return 0;
}
