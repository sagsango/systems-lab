/*
 *
 *
 * TODO: open /dev/console and do ioctl on it!!
 */

/*
42-device ❱❱❱ ls -l /dev/char
total 0
lrwxrwxrwx 1 root root  8 Apr 30 14:47 10:1 -> ../psaux
lrwxrwxrwx 1 root root 21 Apr 30 14:47 10:123 -> ../infiniband/rdma_cm
lrwxrwxrwx 1 root root 18 Apr 30 14:47 10:124 -> ../cpu_dma_latency
lrwxrwxrwx 1 root root 10 Apr 30 14:47 10:125 -> ../udmabuf
lrwxrwxrwx 1 root root 11 Apr 30 14:47 10:126 -> ../ecryptfs
lrwxrwxrwx 1 root root 14 Apr 30 14:47 10:127 -> ../vga_arbiter
lrwxrwxrwx 1 root root  8 Apr 30 14:47 10:183 -> ../hwrng
lrwxrwxrwx 1 root root 12 Apr 30 14:47 10:196 -> ../vfio/vfio
lrwxrwxrwx 1 root root 10 Apr 30 14:47 10:200 -> ../net/tun
lrwxrwxrwx 1 root root  7 Apr 30 14:47 10:203 -> ../cuse
lrwxrwxrwx 1 root root  9 Apr 30 14:47 10:223 -> ../uinput
lrwxrwxrwx 1 root root  9 Apr 30 14:47 10:227 -> ../mcelog
lrwxrwxrwx 1 root root  7 Apr 30 14:47 10:228 -> ../hpet
lrwxrwxrwx 1 root root  7 Apr 30 14:47 10:229 -> ../fuse
lrwxrwxrwx 1 root root 11 Apr 30 14:47 10:231 -> ../snapshot
lrwxrwxrwx 1 root root  6 Apr 30 14:47 10:232 -> ../kvm
lrwxrwxrwx 1 root root 16 Apr 30 14:47 10:234 -> ../btrfs-control
lrwxrwxrwx 1 root root  9 Apr 30 14:47 10:235 -> ../autofs
lrwxrwxrwx 1 root root 17 Apr 30 14:47 10:236 -> ../mapper/control
lrwxrwxrwx 1 root root 15 Apr 30 14:47 10:237 -> ../loop-control
lrwxrwxrwx 1 root root  9 Apr 30 14:47 10:242 -> ../rfkill
lrwxrwxrwx 1 root root  6 Apr 30 14:47 108:0 -> ../ppp
lrwxrwxrwx 1 root root  6 Apr 30 14:47 1:1 -> ../mem
lrwxrwxrwx 1 root root  7 Apr 30 14:47 1:11 -> ../kmsg
lrwxrwxrwx 1 root root  7 Apr 30 14:47 1:3 -> ../null
lrwxrwxrwx 1 root root 15 Apr 26 19:02 13:32 -> ../input/mouse0
lrwxrwxrwx 1 root root 13 Apr 26 19:02 13:63 -> ../input/mice
lrwxrwxrwx 1 root root 15 Apr 26 19:02 13:64 -> ../input/event0
lrwxrwxrwx 1 root root 15 Apr 26 19:02 13:65 -> ../input/event1
lrwxrwxrwx 1 root root 15 Apr 26 19:02 13:66 -> ../input/event2
lrwxrwxrwx 1 root root  7 Apr 30 14:47 1:4 -> ../port
lrwxrwxrwx 1 root root  7 Apr 30 14:47 1:5 -> ../zero
lrwxrwxrwx 1 root root  7 Apr 30 14:47 1:7 -> ../full
lrwxrwxrwx 1 root root  9 Apr 30 14:47 1:8 -> ../random
lrwxrwxrwx 1 root root 18 Apr 30 14:47 189:0 -> ../bus/usb/001/001
lrwxrwxrwx 1 root root 18 Apr 30 14:47 189:1 -> ../bus/usb/001/002
lrwxrwxrwx 1 root root 18 Apr 30 14:47 189:128 -> ../bus/usb/002/001
lrwxrwxrwx 1 root root 18 Apr 30 14:47 189:2 -> ../bus/usb/001/003
lrwxrwxrwx 1 root root 10 Apr 30 14:47 1:9 -> ../urandom
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:0 -> ../cpu/0/msr
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:1 -> ../cpu/1/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:10 -> ../cpu/10/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:11 -> ../cpu/11/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:12 -> ../cpu/12/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:13 -> ../cpu/13/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:14 -> ../cpu/14/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:15 -> ../cpu/15/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:16 -> ../cpu/16/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:17 -> ../cpu/17/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:18 -> ../cpu/18/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:19 -> ../cpu/19/msr
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:2 -> ../cpu/2/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:20 -> ../cpu/20/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:21 -> ../cpu/21/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:22 -> ../cpu/22/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:23 -> ../cpu/23/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:24 -> ../cpu/24/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:25 -> ../cpu/25/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:26 -> ../cpu/26/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:27 -> ../cpu/27/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:28 -> ../cpu/28/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:29 -> ../cpu/29/msr
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:3 -> ../cpu/3/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:30 -> ../cpu/30/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:31 -> ../cpu/31/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:32 -> ../cpu/32/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:33 -> ../cpu/33/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:34 -> ../cpu/34/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:35 -> ../cpu/35/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:36 -> ../cpu/36/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:37 -> ../cpu/37/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:38 -> ../cpu/38/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:39 -> ../cpu/39/msr
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:4 -> ../cpu/4/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:40 -> ../cpu/40/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:41 -> ../cpu/41/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:42 -> ../cpu/42/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:43 -> ../cpu/43/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:44 -> ../cpu/44/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:45 -> ../cpu/45/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:46 -> ../cpu/46/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:47 -> ../cpu/47/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:48 -> ../cpu/48/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:49 -> ../cpu/49/msr
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:5 -> ../cpu/5/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:50 -> ../cpu/50/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:51 -> ../cpu/51/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:52 -> ../cpu/52/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:53 -> ../cpu/53/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:54 -> ../cpu/54/msr
lrwxrwxrwx 1 root root 13 Apr 30 14:47 202:55 -> ../cpu/55/msr
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:6 -> ../cpu/6/msr
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:7 -> ../cpu/7/msr
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:8 -> ../cpu/8/msr
lrwxrwxrwx 1 root root 12 Apr 30 14:47 202:9 -> ../cpu/9/msr
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:0 -> ../cpu/0/cpuid
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:1 -> ../cpu/1/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:10 -> ../cpu/10/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:11 -> ../cpu/11/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:12 -> ../cpu/12/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:13 -> ../cpu/13/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:14 -> ../cpu/14/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:15 -> ../cpu/15/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:16 -> ../cpu/16/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:17 -> ../cpu/17/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:18 -> ../cpu/18/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:19 -> ../cpu/19/cpuid
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:2 -> ../cpu/2/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:20 -> ../cpu/20/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:21 -> ../cpu/21/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:22 -> ../cpu/22/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:23 -> ../cpu/23/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:24 -> ../cpu/24/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:25 -> ../cpu/25/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:26 -> ../cpu/26/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:27 -> ../cpu/27/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:28 -> ../cpu/28/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:29 -> ../cpu/29/cpuid
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:3 -> ../cpu/3/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:30 -> ../cpu/30/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:31 -> ../cpu/31/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:32 -> ../cpu/32/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:33 -> ../cpu/33/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:34 -> ../cpu/34/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:35 -> ../cpu/35/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:36 -> ../cpu/36/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:37 -> ../cpu/37/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:38 -> ../cpu/38/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:39 -> ../cpu/39/cpuid
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:4 -> ../cpu/4/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:40 -> ../cpu/40/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:41 -> ../cpu/41/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:42 -> ../cpu/42/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:43 -> ../cpu/43/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:44 -> ../cpu/44/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:45 -> ../cpu/45/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:46 -> ../cpu/46/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:47 -> ../cpu/47/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:48 -> ../cpu/48/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:49 -> ../cpu/49/cpuid
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:5 -> ../cpu/5/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:50 -> ../cpu/50/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:51 -> ../cpu/51/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:52 -> ../cpu/52/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:53 -> ../cpu/53/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:54 -> ../cpu/54/cpuid
lrwxrwxrwx 1 root root 15 Apr 30 14:47 203:55 -> ../cpu/55/cpuid
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:6 -> ../cpu/6/cpuid
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:7 -> ../cpu/7/cpuid
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:8 -> ../cpu/8/cpuid
lrwxrwxrwx 1 root root 14 Apr 30 14:47 203:9 -> ../cpu/9/cpuid
lrwxrwxrwx 1 root root  6 Apr 30 14:47 21:0 -> ../sg0
lrwxrwxrwx 1 root root 12 Apr 30 14:47 226:0 -> ../dri/card0
lrwxrwxrwx 1 root root 19 Apr 30 14:47 231:0 -> ../infiniband/umad0
lrwxrwxrwx 1 root root 19 Apr 30 14:47 231:1 -> ../infiniband/umad1
lrwxrwxrwx 1 root root 21 Apr 30 14:47 231:192 -> ../infiniband/uverbs0
lrwxrwxrwx 1 root root 21 Apr 30 14:47 231:193 -> ../infiniband/uverbs1
lrwxrwxrwx 1 root root 19 Apr 30 14:47 231:64 -> ../infiniband/issm0
lrwxrwxrwx 1 root root 19 Apr 30 14:47 231:65 -> ../infiniband/issm1
lrwxrwxrwx 1 root root  8 Apr 30 14:47 235:0 -> ../ipmi0
lrwxrwxrwx 1 root root 10 Apr 30 14:47 237:0 -> ../hidraw0
lrwxrwxrwx 1 root root 10 Apr 30 14:47 237:1 -> ../hidraw1
lrwxrwxrwx 1 root root 14 Apr 30 14:47 243:0 -> ../bsg/0:0:0:0
lrwxrwxrwx 1 root root  7 Apr 30 14:47 246:0 -> ../ptp0
lrwxrwxrwx 1 root root  7 Apr 30 14:47 246:1 -> ../ptp1
lrwxrwxrwx 1 root root  7 Apr 30 14:47 246:2 -> ../ptp2
lrwxrwxrwx 1 root root  7 Apr 30 14:47 246:3 -> ../ptp3
lrwxrwxrwx 1 root root  7 Apr 30 14:47 248:0 -> ../rtc0
lrwxrwxrwx 1 root root 18 Apr 30 14:47 249:0 -> ../dma_heap/system
lrwxrwxrwx 1 root root  6 Apr 30 14:47 29:0 -> ../fb0
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:0 -> ../tty0
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:1 -> ../tty1
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:10 -> ../tty10
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:11 -> ../tty11
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:12 -> ../tty12
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:13 -> ../tty13
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:14 -> ../tty14
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:15 -> ../tty15
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:16 -> ../tty16
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:17 -> ../tty17
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:18 -> ../tty18
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:19 -> ../tty19
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:2 -> ../tty2
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:20 -> ../tty20
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:21 -> ../tty21
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:22 -> ../tty22
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:23 -> ../tty23
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:24 -> ../tty24
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:25 -> ../tty25
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:26 -> ../tty26
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:27 -> ../tty27
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:28 -> ../tty28
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:29 -> ../tty29
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:3 -> ../tty3
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:30 -> ../tty30
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:31 -> ../tty31
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:32 -> ../tty32
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:33 -> ../tty33
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:34 -> ../tty34
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:35 -> ../tty35
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:36 -> ../tty36
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:37 -> ../tty37
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:38 -> ../tty38
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:39 -> ../tty39
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:4 -> ../tty4
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:40 -> ../tty40
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:41 -> ../tty41
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:42 -> ../tty42
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:43 -> ../tty43
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:44 -> ../tty44
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:45 -> ../tty45
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:46 -> ../tty46
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:47 -> ../tty47
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:48 -> ../tty48
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:49 -> ../tty49
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:5 -> ../tty5
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:50 -> ../tty50
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:51 -> ../tty51
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:52 -> ../tty52
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:53 -> ../tty53
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:54 -> ../tty54
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:55 -> ../tty55
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:56 -> ../tty56
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:57 -> ../tty57
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:58 -> ../tty58
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:59 -> ../tty59
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:6 -> ../tty6
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:60 -> ../tty60
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:61 -> ../tty61
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:62 -> ../tty62
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:63 -> ../tty63
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:64 -> ../ttyS0
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:65 -> ../ttyS1
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:66 -> ../ttyS2
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:67 -> ../ttyS3
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:68 -> ../ttyS4
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:69 -> ../ttyS5
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:7 -> ../tty7
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:70 -> ../ttyS6
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:71 -> ../ttyS7
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:72 -> ../ttyS8
lrwxrwxrwx 1 root root  8 Apr 30 14:47 4:73 -> ../ttyS9
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:74 -> ../ttyS10
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:75 -> ../ttyS11
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:76 -> ../ttyS12
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:77 -> ../ttyS13
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:78 -> ../ttyS14
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:79 -> ../ttyS15
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:8 -> ../tty8
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:80 -> ../ttyS16
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:81 -> ../ttyS17
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:82 -> ../ttyS18
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:83 -> ../ttyS19
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:84 -> ../ttyS20
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:85 -> ../ttyS21
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:86 -> ../ttyS22
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:87 -> ../ttyS23
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:88 -> ../ttyS24
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:89 -> ../ttyS25
lrwxrwxrwx 1 root root  7 Apr 30 14:47 4:9 -> ../tty9
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:90 -> ../ttyS26
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:91 -> ../ttyS27
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:92 -> ../ttyS28
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:93 -> ../ttyS29
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:94 -> ../ttyS30
lrwxrwxrwx 1 root root  9 Apr 30 14:47 4:95 -> ../ttyS31
lrwxrwxrwx 1 root root  6 Apr 30 14:47 5:0 -> ../tty
lrwxrwxrwx 1 root root 10 Apr 30 14:47 5:1 -> ../console
lrwxrwxrwx 1 root root  7 Apr 30 14:47 5:2 -> ../ptmx
lrwxrwxrwx 1 root root 12 Apr 30 14:47 5:3 -> ../ttyprintk
lrwxrwxrwx 1 root root  6 Apr 30 14:47 7:0 -> ../vcs
lrwxrwxrwx 1 root root  7 Apr 30 14:47 7:1 -> ../vcs1
lrwxrwxrwx 1 root root  7 Apr 30 14:47 7:128 -> ../vcsa
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:129 -> ../vcsa1
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:130 -> ../vcsa2
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:131 -> ../vcsa3
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:132 -> ../vcsa4
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:133 -> ../vcsa5
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:134 -> ../vcsa6
lrwxrwxrwx 1 root root  7 Apr 30 14:47 7:2 -> ../vcs2
lrwxrwxrwx 1 root root  7 Apr 30 14:47 7:3 -> ../vcs3
lrwxrwxrwx 1 root root  7 Apr 30 14:47 7:4 -> ../vcs4
lrwxrwxrwx 1 root root  7 Apr 30 14:47 7:5 -> ../vcs5
lrwxrwxrwx 1 root root  7 Apr 30 14:47 7:6 -> ../vcs6
lrwxrwxrwx 1 root root  7 Apr 30 14:47 7:64 -> ../vcsu
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:65 -> ../vcsu1
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:66 -> ../vcsu2
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:67 -> ../vcsu3
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:68 -> ../vcsu4
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:69 -> ../vcsu5
lrwxrwxrwx 1 root root  8 Apr 30 14:47 7:70 -> ../vcsu6
lrwxrwxrwx 1 root root  8 Apr 30 14:47 89:0 -> ../i2c-0
lrwxrwxrwx 1 root root  8 Apr 30 14:47 89:1 -> ../i2c-1
lrwxrwxrwx 1 root root  8 Apr 30 14:47 89:2 -> ../i2c-2
lrwxrwxrwx 1 root root  8 Apr 30 14:47 89:3 -> ../i2c-3
42-device ❱❱❱
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <linux/fs.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/vt.h>  /* Definition of VT_* constants */

#define CHAR_DEVICE "/dev/console"

int32_t main() {
  int32_t fd = open(CHAR_DEVICE, O_RDONLY);
  if (fd < 0) {
    perror("Cant open char device");
  }

  /*
   * 
   * 1. Ioctl ops:
   *    https://elixir.bootlin.com/linux/v4.7/source/include/uapi/asm-generic/ioctls.h#L37
   * 
   */

  
  struct winsize ws;
  if (ioctl(fd, TIOCGWINSZ, &ws) < 0) {
    perror("Error on getting the size of char device - console\n");
  }
  printf("Console windowsize rows:%d, cols:%d\n", ws.ws_row, ws.ws_col);

}