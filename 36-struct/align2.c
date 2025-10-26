#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

struct __attribute__((packed)) Data1 {
    int a;
    char b;
    long c;
    char d;
    int e;
};

struct Data2 {
    int a;
    char b;
    long c;
    char d;
    int e;
};

struct __attribute__((aligned(64))) Data3 {
    int a;
    char b;
    long c;
    char d;
    int e;
};

struct Data4 {
    int a;
    char b;
    long c;
    char d;
    int e;
    long arr[]; // flexible array
};

// Helper macro
#define PRINT_INFO(StructType, Member) \
    printf("%-6s %-8s offset = %-3zu size = %-3zu\n", \
        #StructType, #Member, offsetof(struct StructType, Member), sizeof(((struct StructType *)0)->Member))

int main(void) {
    printf("\n---- Data1 (packed) ----\n");
    PRINT_INFO(Data1, a);
    PRINT_INFO(Data1, b);
    PRINT_INFO(Data1, c);
    PRINT_INFO(Data1, d);
    PRINT_INFO(Data1, e);
    printf("sizeof(Data1) = %zu\n\n", sizeof(struct Data1));

    printf("---- Data2 (normal) ----\n");
    PRINT_INFO(Data2, a);
    PRINT_INFO(Data2, b);
    PRINT_INFO(Data2, c);
    PRINT_INFO(Data2, d);
    PRINT_INFO(Data2, e);
    printf("sizeof(Data2) = %zu\n\n", sizeof(struct Data2));

    printf("---- Data3 (aligned(64)) ----\n");
    PRINT_INFO(Data3, a);
    PRINT_INFO(Data3, b);
    PRINT_INFO(Data3, c);
    PRINT_INFO(Data3, d);
    PRINT_INFO(Data3, e);
    printf("sizeof(Data3) = %zu\n\n", sizeof(struct Data3));

    printf("---- Data4 (flexible array) ----\n");
    PRINT_INFO(Data4, a);
    PRINT_INFO(Data4, b);
    PRINT_INFO(Data4, c);
    PRINT_INFO(Data4, d);
    PRINT_INFO(Data4, e);
    printf("sizeof(Data4) = %zu (before flexible array)\n\n", sizeof(struct Data4));

    // Example of allocating Data4 with extra array
    int n = 5;
    struct Data4 *d4 = malloc(sizeof(struct Data4) + n * sizeof(long));
    printf("Allocated Data4 for %d longs -> total size = %zu bytes\n",
           n, sizeof(struct Data4) + n * sizeof(long));
    d4->arr[4] = 10;
    // 24 | 0 1 2 3 4
    //
    char * addr1 = &(d4->arr[4]);
    char * addr2 = d4;
    printf("%ld\n", addr1 - addr2);
    free(d4);

    return 0;
}

