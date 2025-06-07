#include <stdio.h>
#include <stdint.h>

int main() {
    char arr[] = {0x11, 0x22, 0x33, 0x44, 0x0};

    // Cast the address of arr to an int pointer
    int* ptr = (int*)arr;

    printf("Value pointed to: 0x%x\n", *ptr);
    return 0;
}
/* XXX:
 *
 *  Little endian output: 0x44332211
 *  Big endian output   : 0x11223344
 */
