#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define FILENAME "mmap-multi.txt"
#define PAGE_SIZE (1 << 12)

void read_file(int fd) {
  if (lseek(fd, 0, SEEK_SET)) {
    perror("seek set failed");
  }
  long num = 0;
  if (read(fd, &num, sizeof(num)) != sizeof(num)) {
    perror("Cant read file");
  }
  printf("File-read: %p\n", (void *)num);
}

int main() {
  int fd = open(FILENAME, O_CREAT | O_RDWR | O_EXCL,
                S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXUSR |
                    S_IROTH | S_IWOTH | S_IXOTH);

  if (fd < 0) {
    perror("Cant create file");
  }

  if (ftruncate(fd, PAGE_SIZE) < 0) {
    perror("Cant truncate");
  }

  void *addr1 =
      mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, fd, 0);
  if (addr1 == MAP_FAILED) {
    perror("mmap1 failed");
  }

  *((long *)addr1) = 0xbadbeef;
  printf("addr1:%p\n", *((long *)addr1));

  void *addr2 =
      mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, fd, 0);
  if (addr2 == MAP_FAILED) {
    perror("mmap2 failed");
  }
  printf("addr2:%p\n", *((long *)addr2));

  void *addr3 =
      mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FILE, fd, 0);
  if (addr3 == MAP_FAILED) {
    perror("mmap3 failed");
  }

  printf("addr3:%p\n", *((long *)addr3));

  *((long *)addr3) = 0xbadbad;

  printf("addr1:%p\n", *((long *)addr1));
  printf("addr2:%p\n", *((long *)addr2));
  printf("addr3:%p\n", *((long *)addr3));

/* Link: https://man7.org/linux/man-pages/man2/msync.2.html
 * NOTE: This mapping should be shared (otherwise we cant sync)
   msync() flushes changes made to the in-core copy of a file that
       was mapped into memory using mmap(2) back to the filesystem.
       Without use of this call, there is no guarantee that changes are
       written back before munmap(2) is called.  To be more precise, the
       part of the file that corresponds to the memory area starting at
       addr and having length length is updated.

       The flags argument should specify exactly one of MS_ASYNC and
       MS_SYNC, and may additionally include the MS_INVALIDATE bit.
       These bits have the following meanings:

       MS_ASYNC
              Specifies that an update be scheduled, but the call returns
              immediately.

       MS_SYNC
              Requests an update and waits for it to complete.

       MS_INVALIDATE
              Asks to invalidate other mappings of the same file (so that
              they can be updated with the fresh values just written).
 */

  read_file(fd);
  if (msync(addr1, PAGE_SIZE, MS_SYNC) < 0) {
    perror("Cant sync the mmap");
  }
  read_file(fd);
  printf("NOTE: We cant msync the private mmaping\n");
  if (msync(addr3, PAGE_SIZE, MS_SYNC) < 0) {
    perror("Cant sync the mmap");
  }
  read_file(fd);
  if (msync(addr2, PAGE_SIZE, MS_SYNC) < 0) {
    perror("Cant sync the mmap");
  }
  read_file(fd);

  if (close(fd) < 0) {
    perror("Cant close");
  }

  if (unlink(FILENAME)) {
    perror("Cant unlink");
  }
}