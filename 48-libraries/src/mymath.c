#include "mymath.h"
#include <stdio.h>

int add(int a, int b) {
    printf("Adding %d and %d\n", a, b);
    return a + b;
}

int subtract(int a, int b) {
    printf("Subtracting %d from %d\n", b, a);
    return a - b;
}
