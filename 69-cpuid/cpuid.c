#include <unistd.h>

int main(void) {
    unsigned int eax, ebx, ecx, edx;
    char vendor[13];

    eax = 0;
    __asm__ volatile (
        "cpuid"
        : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
        : "a"(eax)
    );

    vendor[0]  = ebx & 0xff;
    vendor[1]  = (ebx >> 8) & 0xff;
    vendor[2]  = (ebx >> 16) & 0xff;
    vendor[3]  = (ebx >> 24) & 0xff;

    vendor[4]  = edx & 0xff;
    vendor[5]  = (edx >> 8) & 0xff;
    vendor[6]  = (edx >> 16) & 0xff;
    vendor[7]  = (edx >> 24) & 0xff;

    vendor[8]  = ecx & 0xff;
    vendor[9]  = (ecx >> 8) & 0xff;
    vendor[10] = (ecx >> 16) & 0xff;
    vendor[11] = (ecx >> 24) & 0xff;
    vendor[12] = '\n';

    write(1, vendor, 13);
    return 0;
}
