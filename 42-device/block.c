/*
 *
 *
 * TODO:
 *      open device file and do ioctl on it
 */


/*
42-device ❱❱❱ ls -l /dev/block/
total 0
lrwxrwxrwx 1 root root 7 Apr 30 14:47 253:0 -> ../dm-0
lrwxrwxrwx 1 root root 8 May 19 04:11 7:0 -> ../loop0
lrwxrwxrwx 1 root root 8 May  8 12:07 7:1 -> ../loop1
lrwxrwxrwx 1 root root 9 Apr 30 14:47 7:10 -> ../loop10
lrwxrwxrwx 1 root root 9 Apr 30 14:47 7:11 -> ../loop11
lrwxrwxrwx 1 root root 9 Apr 30 14:47 7:12 -> ../loop12
lrwxrwxrwx 1 root root 9 Apr 30 14:47 7:13 -> ../loop13
lrwxrwxrwx 1 root root 9 Apr 30 14:47 7:14 -> ../loop14
lrwxrwxrwx 1 root root 9 May  8 12:07 7:15 -> ../loop15
lrwxrwxrwx 1 root root 8 Apr 30 14:47 7:2 -> ../loop2
lrwxrwxrwx 1 root root 8 Apr 30 14:47 7:3 -> ../loop3
lrwxrwxrwx 1 root root 8 Apr 30 14:47 7:4 -> ../loop4
lrwxrwxrwx 1 root root 8 Apr 30 14:47 7:5 -> ../loop5
lrwxrwxrwx 1 root root 8 Apr 30 14:47 7:6 -> ../loop6
lrwxrwxrwx 1 root root 8 Apr 30 14:47 7:7 -> ../loop7
lrwxrwxrwx 1 root root 8 Apr 30 14:47 7:8 -> ../loop8
lrwxrwxrwx 1 root root 8 Apr 30 14:47 7:9 -> ../loop9
lrwxrwxrwx 1 root root 6 May 14 06:04 8:0 -> ../sda
lrwxrwxrwx 1 root root 7 Apr 30 14:47 8:1 -> ../sda1
lrwxrwxrwx 1 root root 7 Apr 30 14:47 8:2 -> ../sda2
lrwxrwxrwx 1 root root 7 Apr 30 14:47 8:3 -> ../sda3
42-device ❱❱❱
*/



#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <linux/fs.h>
#include <stdlib.h>
#include <sys/ioctl.h>


#define BLOCK_DEVICE "/dev/sda"

int32_t main() {
  int32_t fd = open(BLOCK_DEVICE, O_RDWR);
  if (fd < 0) {
    perror("Cant open block device");
  }

  /*
   * 
   * 1. ioctl operation:
   *    https://elixir.bootlin.com/linux/v4.7/source/include/uapi/linux/fs.h#L211
   * 
   * 
   * 2. test it:
   *     sudo blockdev --getsize64 /dev/sda
   *     root@quads2:/home/ssing214/systems-lab/42-device# sudo blockdev --getsize64 /dev/sda
   */



/*
  root@quads2:/home/ssing214/systems-lab/42-device# blockdev --help

  Usage:
  blockdev [-v|-q] commands devices
  blockdev --report [devices]
  blockdev -h|-V

  Call block device ioctls from the command line.

  Options:
  -q             quiet mode
  -v             verbose mode
      --report   print report for specified (or all) devices

  -h, --help     display this help
  -V, --version  display version

  Available commands:
  --getsz                   get size in 512-byte sectors
  --setro                   set read-only
  --setrw                   set read-write
  --getro                   get read-only
  --getdiscardzeroes        get discard zeroes support status
  --getss                   get logical block (sector) size
  --getpbsz                 get physical block (sector) size
  --getiomin                get minimum I/O size
  --getioopt                get optimal I/O size
  --getalignoff             get alignment offset in bytes
  --getmaxsect              get max sectors per request
  --getbsz                  get blocksize
  --setbsz <bytes>          set blocksize on file descriptor opening the block device
  --getsize                 get 32-bit sector count (deprecated, use --getsz)
  --getsize64               get size in bytes
  --setra <sectors>         set readahead
  --getra                   get readahead
  --setfra <sectors>        set filesystem readahead
  --getfra                  get filesystem readahead
  --flushbufs               flush buffers
  --rereadpt                reread partition table

  For more details see blockdev(8).
*/



  uint64_t size = 0;
  if (ioctl(fd, BLKGETSIZE64, &size) < 0) {
    perror("Cant do the ioctl for size of block device\n");
  }
  printf("Size of the block device: %lu\n", size);

  if (close(fd) < 0) {
    perror("Cant close block device file\n");
  }



}
