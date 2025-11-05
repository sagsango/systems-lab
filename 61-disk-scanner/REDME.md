Given the device file (how the disk are exposed to userspace), it will scan the whole disk and print the disk memtatda
1. Superblock info
2. Inode Bit Map
3. Block Bitmaps
4. all the inodes and blocks owned by the inode
5. etc

NOTE: Like here in ext4, vfat, and squashfs. We need FSTYPE spesific program. 
NOTE: There are already tools available for it ex:  sudo dumpe2fs /dev/nvme0n1p2

61-disk-scanner ❱❱❱ lsblk -f
NAME        FSTYPE   FSVER LABEL UUID                                 FSAVAIL FSUSE% MOUNTPOINTS
loop0       squashfs 4.0                                                    0   100% /snap/bare/5
loop1       squashfs 4.0                                                    0   100% /snap/core/17247
loop2       squashfs 4.0                                                    0   100% /snap/core22/2133
loop3       squashfs 4.0                                                    0   100% /snap/firefox/7177
loop4       squashfs 4.0                                                    0   100% /snap/firefox/7084
loop5       squashfs 4.0                                                    0   100% /snap/snapd/25577
loop6       squashfs 4.0                                                    0   100% /snap/firmware-updater/167
loop7       squashfs 4.0                                                    0   100% /snap/gnome-42-2204/226
loop8       squashfs 4.0                                                    0   100% /snap/gnome-42-2204/202
loop9       squashfs 4.0                                                    0   100% /snap/kf6-core22/43
loop10      squashfs 4.0                                                    0   100% /snap/gtk-common-themes/1535
loop11      squashfs 4.0                                                    0   100% /snap/snap-store/1300
loop12      squashfs 4.0                                                    0   100% /snap/snap-store/1270
loop13      squashfs 4.0                                                    0   100% /snap/core22/2139
loop14      squashfs 4.0                                                    0   100% /snap/snapd-desktop-integration/253
loop15      squashfs 4.0                                                    0   100% /snap/snapd/25202
loop16      squashfs 4.0                                                    0   100% /snap/usbutils-snap/2
loop17      squashfs 4.0                                                    0   100% /snap/snapd-desktop-integration/315
loop18      squashfs 4.0                                                    0   100% /snap/usbutils-lool/2
loop19      squashfs 4.0                                                    0   100% /snap/firmware-updater/210
nvme0n1
├─nvme0n1p1 vfat     FAT32       E592-C4A9                                 1G     1% /boot/efi
└─nvme0n1p2 ext4     1.0         600d35e3-6287-4359-bd92-b90ef612b75d   42.2G    58% /
