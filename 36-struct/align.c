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
    long arr[]; /* Variable size array; will be last member; cant create array of this struct */
};


int main() {

    struct Data1 d;


    int n = 5; // number of longs; in the veriable size array
    struct Data4 *d4 = malloc(sizeof(struct Data4) + n * sizeof(long));


    struct Data4 arrs[10]; /* XXX: Undefined behavior — arr[] has unknown size */

}