//TODO:


/*


Futext is the fastest. (sem_t, pthread_cond_t)


futex (Fast User-Space Mutex):
A futex is a lightweight synchronization primitive tied to a 32-bit integer in user-space memory, managed by the kernel only when contention occurs.
The futex() system call is the interface, with operations like FUTEX_WAIT and FUTEX_WAKE.
Kernel Implementation
User-Space Fast Path:
In the uncontended case, futex operations (e.g., lock/unlock) use atomic instructions (e.g., cmpxchg) entirely in user space, avoiding kernel entry.
The kernel isn’t involved unless a system call is needed (e.g., for waiting or waking).
System Call Entry:
The futex() syscall (via sys_futex or do_futex) takes a user-space address (uaddr), operation (op), and value (val) as arguments.
Example: futex(uaddr, FUTEX_WAIT, val, timeout) or futex(uaddr, FUTEX_WAKE, val).

*/
