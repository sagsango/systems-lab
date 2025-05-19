#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>

#define FILENAME "mmap-multi.txt"
#define PAGE_SIZE (1<<12)
int main() {
    int fd = open(FILENAME, O_CREAT | O_RDWR | O_EXCL,
        S_IRUSR | S_IWUSR | S_IXUSR  |
        S_IRGRP | S_IWGRP | S_IXUSR  |
        S_IROTH | S_IWOTH | S_IXOTH);

    if (fd < 0) {
        perror("Cant create file");
    }

    if (ftruncate(fd, PAGE_SIZE) < 0) {
        perror("Cant truncate");
    }

    void * addr1 = mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED | MAP_FILE, fd,  0);
    if (addr1 == MAP_FAILED) {
        perror("mmap1 failed");
    }

    *((long*)addr1) = 0xbadbeef;
    printf("addr1:%p\n", *((long*)addr1));


   void * addr2= mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED | MAP_FILE, fd,  0); 
    if (addr2 == MAP_FAILED) {
        perror("mmap2 failed");
    }
    printf("addr2:%p\n", *((long*)addr2));

   void * addr3= mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_FILE, fd,  0);  
    if (addr3 == MAP_FAILED) {
        perror("mmap3 failed");
    }

    printf("addr3:%p\n", *((long*)addr3)); 

    *((long*)addr3) = 0xbadbad;

    printf("addr1:%p\n", *((long*)addr1));
    printf("addr2:%p\n", *((long*)addr2));
    printf("addr3:%p\n", *((long*)addr3)); 
    

    if (close(fd) < 0) {
        perror("Cant close");
    }

    if (unlink(FILENAME)) {
        perror("Cant unlink");
    }
}