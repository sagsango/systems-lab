#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

int *init = NULL;

/*XXX: load time construction */
__attribute__((constructor))
static void lib_init(void)
{
    int i;

    printf("[mylib] constructor: lib_init()\n");

    init = malloc(10 * sizeof(int));
    if (!init) {
        fprintf(stderr, "[mylib] malloc failed\n");
        return;
    }

    for (i = 0; i < 10; i++)
        init[i] = i + 1;
}

__attribute__((destructor))
static void lib_fini(void)
{
    printf("[mylib] destructor: lib_fini()\n");

    free(init);
    init = NULL;
}

void print_array(void)
{
    int i;

    if (!init) {
        printf("[mylib] init is NULL\n");
        return;
    }

    printf("[mylib] init array: ");
    for (i = 0; i < 10; i++)
        printf("%d ", init[i]);
    printf("\n");
}
