#include <stdlib.h>
#include <assert.h>


#define PAGE_SIZE (1<<12)
#define ALIGN (PAGE_SIZE*2)
#define SIZE (PAGE_SIZE*16)
int main() {
    void * addr = aligned_alloc(ALIGN, SIZE);

    assert (( (long)addr&(ALIGN-1)) == 0);
    /*
     * alloc init the memory by 0
     */

    void * ptr = addr;
    char * c;

    while (ptr < addr + SIZE) {
        c = (char*)ptr;
        assert(*c == 0);
        c += 1;
        addr = c;
    }

    /*
     * 
     * 
     * TODO:
     *  free() the memeory to avoid memory leaks
    */

   // int x = rdtscp();
}