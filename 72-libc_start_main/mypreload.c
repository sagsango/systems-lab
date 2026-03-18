#define _GNU_SOURCE // Required for RTLD_NEXT
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

// Define the type for the original __libc_start_main function
typedef int (*orig_libc_start_main_t)(
    int (*main)(int, char **, char **),
    int argc,
    char **argv,
    void (*init)(void),
    void (*fini)(void),
    void (*rtld_fini)(void),
    void *stack_end);

// Define a 'fake' main function where you can put your custom code
int fake_main(int argc, char **argv, char **envp) {
    printf("--- My preloaded code is running! ---\n");
    printf("Program name: %s\n", argv[0]);
    // You can add your custom logic here

    // Find and call the real main function
    orig_libc_start_main_t orig_func;
    orig_func = (orig_libc_start_main_t) dlsym(RTLD_NEXT, "__libc_start_main");

    // The *actual* main function pointer is often passed as the first argument
    // to our hooked __libc_start_main. We need to pass the *real* main 
    // to the *real* __libc_start_main. This is complex and platform-dependent.

    // A common and simpler approach is to get a pointer to the *real* 
    // main from the arguments passed to our hooked function and call it 
    // directly from within a *wrapper* function.
    // For simplicity here, we assume we can call the original main function 
    // directly. In reality, the setup is more involved as shown in some 
    // linked examples.

    // This simple direct call to 'main' is illustrative and may not work 
    // in all scenarios due to the complexity of the glibc startup routine.
    // A more robust method involves creating a wrapper function to eventually 
    // call the real main, as the real __libc_start_main sets up the C runtime 
    // environment.

    // Example of the more common 'wrapper' strategy:
    // You would typically store the real main's address and return a pointer 
    // to a 'fake_main' to the real __libc_start_main. The real __libc_start_main 
    // then calls your 'fake_main', which then calls the real 'main'.

    // For demonstration, here is the basic structure:
    // The actual implementation details can be found in specialized examples 
    // linked in the search results.

    // ... (complex wrapper logic from external sources needed here) ...

    return 0; // Or return the result of calling the real main
}

// The function signature needs to match the real __libc_start_main
int __libc_start_main(
    int (*main_func)(int, char **, char **),
    int argc,
    char **argv,
    void (*init_func)(void),
    void (*fini_func)(void),
    void (*rtld_fini_func)(void),
    void *stack_end) {

    // In a proper implementation, you would save main_func, and pass 
    // a pointer to a fake main to the *original* __libc_start_main
    // which you'd find using dlsym(RTLD_NEXT, "__libc_start_main").

    // The logic is complex because the real __libc_start_main handles critical 
    // environment setup you cannot easily replicate.

    // For a working example, refer to specific code on [GitHub](https://github.com/thomasnyman/abort_at_exit) or [Malware Hell](https://c3rb3ru5d3d53c.github.io/2023/02/hooking-libc.en.md/)

    // This is a complex technique used in rootkits and security research.

    // The most reliable way is often to use the `__attribute__((constructor))` 
    // in your library to run code *when the library is loaded*.
    // This runs before the real __libc_start_main returns to call main.

    // Example using constructor attribute (simpler):
    /*
    void my_constructor(void) __attribute__((constructor));

    void my_constructor(void) {
        printf("--- Constructor code is running before main! ---\n");
    }
    */

    // Calling the real __libc_start_main is necessary for program to run.
    orig_libc_start_main_t orig_start_main;
    orig_start_main = (orig_libc_start_main_t) dlsym(RTLD_NEXT, "__libc_start_main");
    printf("Fake __libc_start_main. <START>\n");

    orig_start_main(main_func, argc, argv, init_func, fini_func, rtld_fini_func, stack_end);

    printf("Fake __libc_start_main. <DONE>\n");

    return 0;
}

