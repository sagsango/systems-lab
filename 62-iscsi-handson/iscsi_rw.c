#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEVICE_PATH "/dev/sda"  // Replace with your actual iSCSI device (e.g., from lsblk)
#define BUFFER_SIZE 512
#define WRITE_DATA "Hello, iSCSI!"
#define OFFSET 0  // Start at beginning; adjust if formatted

int main() {
    int fd;
    char read_buffer[BUFFER_SIZE];
    ssize_t bytes_written, bytes_read;

    // Open the device
    fd = open(DEVICE_PATH, O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("Failed to open device");
        exit(EXIT_FAILURE);
    }

    // Write data
    bytes_written = pwrite(fd, WRITE_DATA, strlen(WRITE_DATA) + 1, OFFSET);
    if (bytes_written == -1) {
        perror("Failed to write");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Wrote %zd bytes: %s\n", bytes_written, WRITE_DATA);

    // Read data back
    bytes_read = pread(fd, read_buffer, BUFFER_SIZE, OFFSET);
    if (bytes_read == -1) {
        perror("Failed to read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    read_buffer[bytes_read] = '\0';  // Null-terminate for printing
    printf("Read %zd bytes: %s\n", bytes_read, read_buffer);

    // Close the device
    close(fd);
    return 0;
}
