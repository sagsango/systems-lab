#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef void (*print_array_fn)(void);

int main(void)
{
    void *handle;
    print_array_fn print_array;

    printf("[dlmain] before dlopen()\n");

    handle = dlopen("./libmylib.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        return 1;
    }

    printf("[dlmain] after dlopen()\n");

    *(void **)(&print_array) = dlsym(handle, "print_array");
    if (!print_array) {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    print_array();

    printf("[dlmain] before dlclose()\n");
    dlclose(handle);
    printf("[dlmain] after dlclose()\n");

    return 0;
}
