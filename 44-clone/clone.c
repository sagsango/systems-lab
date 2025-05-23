#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <stdint.h>
#include <sched.h>
#include <linux/sched.h>    /* Definition of struct clone_args */
#include <sched.h>          /* Definition of CLONE_* constants */
#include <sys/syscall.h>    /* Definition of SYS_* constants */
#include <unistd.h>


int main() {

/*
https://man7.org/linux/man-pages/man2/clone.2.html
clone:

       int clone(typeof(int (void *_Nullable)) *fn,
                 void *stack,
                 int flags,
                 void *_Nullable arg, ...
                 // pid_t *_Nullable parent_tid,
                 //   void *_Nullable tls,
                 //   pid_t *_Nullable child_tid
                 );

*/




 struct clone_args args;
/*
https://man7.org/linux/man-pages/man2/clone.2.html
 clone3:
 
 long syscall(SYS_clone3, struct clone_args *cl_args, size_t size);

 struct clone_args {
               u64 flags;        // Flags bit mask : CLONE_
               u64 pidfd;        // Where to store PID file descriptor (int *) 
               u64 child_tid;    // Where to store child TID, in child's memory (pid_t *)
               u64 parent_tid;   // Where to store child TID,
                                    in parent's memory (pid_t *) 
               u64 exit_signal;  // Signal to deliver to parent on child termination 
               u64 stack;        // Pointer to lowest byte of stack 
               u64 stack_size;   // Size of stack 
               u64 tls;          // Location of new TLS 
               u64 set_tid;      // Pointer to a pid_t array since Linux 5.5) 
               u64 set_tid_size; // Number of elements in set_tid (since Linux 5.5) 
               u64 cgroup;       // File descriptor for target cgroup of child (since Linux 5.7) 
           };
*/

/*
XXX: For new namespace or containers
//CLONE_NEWCGROUP
//CLONE_NEWIPC
//CLONE_NEWNET
//CLONE_NEWNS
//CLONE_NEWPID
//CLONE_NEWTIME
//CLONE_NEWUSER
//CLONE_NEWUTS
*/


//CLONE_CLEAR_SIGHAND   : Child does not copy parent's signal handler; they are default.
//CLONE_SIGHAND         : Child points to parent's signal handler. So sigaction(2) on one affects the other too.
//CLONE_FILES           : Parent and child share the same file-descripter table. (FD-table is same)
//CLONE_FS              : Parent and child have same copy of file systems info (root, cwd etc). So chroot, chdir in one also affects other.
//CLONE_IO              : 
//CLONE_PARENT
//CLONE_THREAD          : Child have same thread group as parent; they share parents too! (Parent will get SIGCHILD on exit of this child, and also wait will work.
//                        only for the creaters thread parent, as creater thread will get the SIGCHILD).
//                        if any of the threads do exec() all the threads except thread-group leader get terminated, and new program is executed in thread group leader.
//CLONE_VFORK           : calling process get suspended untill child releases its virtual memory by calling exec(), exit(). - Same behaviour as vfork().
//CLONE_VM              : calling process and child share shame virual-space or page-table or mm_struct(mmaped-file, mmaped-device etc)
}