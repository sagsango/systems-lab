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

/*
 * XXX: From FreeBSD
typedef struct __ucontext {
     *
     * Keep the order of the first two fields. Also,
     * keep them the first two fields in the structure.
     * This way we can have a union with struct
     * sigcontext and ucontext_t. This allows us to
     * support them both at the same time.
     * note: the union is not defined, though.
     *
    __sigset_t  uc_sigmask;
    mcontext_t  uc_mcontext;

    struct __ucontext *uc_link;
    struct __stack_t uc_stack;
    int     uc_flags;
    int     __spare__[4];
} ucontext_t;


typedef struct __mcontext {
    *
     * The definition of mcontext_t must match the layout of
     * struct sigcontext after the sc_mask member.  This is so
     * that we can support sigcontext and ucontext_t at the same
     * time.
     *
    __register_t    mc_onstack; * XXX - sigcontext compat. *
    __register_t    mc_rdi;     * machine state (struct trapframe) *
    __register_t    mc_rsi;
    __register_t    mc_rdx;
    __register_t    mc_rcx;
    __register_t    mc_r8;
    __register_t    mc_r9;
    __register_t    mc_rax;
    __register_t    mc_rbx;
    __register_t    mc_rbp;
    __register_t    mc_r10;
    __register_t    mc_r11;
    __register_t    mc_r12;
    __register_t    mc_r13;
    __register_t    mc_r14;
    __register_t    mc_r15;
    __uint32_t  mc_trapno;
    __uint16_t  mc_fs;
    __uint16_t  mc_gs;
    __register_t    mc_addr;
    __uint32_t  mc_flags;
    __uint16_t  mc_es;
    __uint16_t  mc_ds;
    __register_t    mc_err;
    __register_t    mc_rip;
    __register_t    mc_cs;
    __register_t    mc_rflags;
    __register_t    mc_rsp;
    __register_t    mc_ss;

    long    mc_len;         * sizeof(mcontext_t) *

#define _MC_FPFMT_NODEV     0x10000 * device not present or configured *
#define _MC_FPFMT_XMM       0x10002
    long    mc_fpformat;
#define _MC_FPOWNED_NONE    0x20000 * FP state not used *
#define _MC_FPOWNED_FPU     0x20001 * FP state came from FPU *
#define _MC_FPOWNED_PCB     0x20002 * FP state came from PCB *
    long    mc_ownedfp;
     *
     * See <machine/fpu.h> for the internals of mc_fpstate[].
     *
    long    mc_fpstate[64] __aligned(16);

    __register_t    mc_fsbase;
    __register_t    mc_gsbase;

    __register_t    mc_xfpustate;
    __register_t    mc_xfpustate_len;

    long    mc_spare[4];
} mcontext_t;
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
